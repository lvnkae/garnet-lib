/*!
 *  @file   stock_ordering_manager.cpp
 *  @brief  �������Ǘ���
 *  @date   2017/12/26
 */
#include "stock_ordering_manager.h"

#include "securities_session.h"
#include "stock_trading_command.h"
#include "stock_trading_tactics.h"
#include "trade_assistant_setting.h"
#include "trade_struct.h"

#include "cipher_aes.h"
#include "utility_datetime.h"

#include <list>
#include <stack>
#include <unordered_map>

namespace trading
{
class StockOrderingManager::PIMPL
{
private:
    /*!
     *  @brief  �������ʏ��
     */
    struct StockOrderIdentifier
    {
        int32_t m_tactics_id;   //!< �헪ID
        int32_t m_group_id;     //!< �헪���O���[�vID

        StockOrderIdentifier(int32_t t, int32_t g)
        : m_tactics_id(t)
        , m_group_id(g)
        {
        }
    };

    //! �،���ЂƂ̃Z�b�V����
    std::shared_ptr<SecuritiesSession> m_pSession;
    //! ���߃��X�g<�����헪id, ����>
    std::list<std::pair<int32_t, StockTradingCommand>> m_command_list;
    //! �ً}���[�h�Ώێ�<�헪id, �c����(�~���b)>
    std::unordered_map<int32_t, int64_t> m_emergency_tactics;
    //! �ً}���[�h����[�~���b] ���O���ݒ肩��擾
    const int64_t m_emergency_time_ms;
    //! �������ʑ҂�Order
    std::stack<std::pair<StockOrderIdentifier, StockOrder>> m_wait_order;
    //! ������<order_id, order>
    std::unordered_map<int32_t, std::pair<StockOrderIdentifier, StockOrder>> m_server_order;
    //! ���݂̑Ώێ����
    eStockInvestmentsType m_investments;
    //! �o�ߎ���[�~���b] ��Interpret�I�����ɍX�V
    int64_t m_tick_count;


    /*!
     *  @brief  ���������R�[���o�b�N
     *  @param  b_result    ����
     *  @param  rcv_order   ��������
     */
    void StockOrderCallback(bool b_result, const RcvResponseStockOrder& rcv_order)
    { 
        if (m_wait_order.empty()) {
            return; // �v���I�ȃG���[(error)
        }
        const auto& w_order = m_wait_order.top();
        const StockOrderIdentifier& soidx = w_order.first;
        const StockOrder& order = w_order.second;
        if (b_result) {
            const float64 diff_value = order.m_value - rcv_order.m_value;
            if (order.m_code.GetCode() == rcv_order.m_code &&
                order.m_type == rcv_order.m_type &&
                order.m_investiments == rcv_order.m_investments &&
                ((order.m_b_leverage && rcv_order.m_b_leverage) || (!order.m_b_leverage && !rcv_order.m_b_leverage)) &&
                (-0.05 < diff_value && diff_value < 0.05) && /* �덷0.05�����͋��e(���������_�̈�v��r�͂�����̂�) */
                order.m_number == rcv_order.m_number)
            {
                m_server_order.emplace(rcv_order.m_order_id, std::pair<StockOrderIdentifier, StockOrder>(soidx, order));
                // >ToDo< ��������������(twitter�ʒm)
            }
        } else {
            // >ToDo< �������s������(twitter�ʒm)
        }
        m_wait_order.pop();
    }

public:
    /*!
     *  @param  session     �،���ЂƂ̃Z�b�V����
     *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
     */
    PIMPL(const std::shared_ptr<SecuritiesSession>& session, const TradeAssistantSetting& script_mng)
    : m_pSession(session)
    , m_command_list()
    , m_emergency_tactics()
    , m_emergency_time_ms(utility::ToMiliSecondsFromSecond(script_mng.GetEmergencyCoolSecond()))
    , m_investments(INVESTMENTS_NONE)
    , m_tick_count(0)
    {
    }

    /*!
     *  @brief  machine�p�����[�^�X�V
     *  @param  tickCount   �o�ߎ���[�~���b]
     *  @param  investments �Ώێ����
     */
    void Correct(int64_t tickCount, eStockInvestmentsType investments)
    {
        m_tick_count = tickCount;
        m_investments = investments;
    }

    /*!
     *  @brief  ��������F�ً}���[�h
     *  @param  tickCount   �o�ߎ���[�~���b]
     *  @param  tactics     ����헪
     *  @retval true    ���Y�헪�ً͋}���[�h
     */
    bool Update_Emergency(int64_t tickCount, const StockTradingTactics& tactics)
    {
        auto it = m_emergency_tactics.find(tactics.GetUniqueID());
        if (it != m_emergency_tactics.end()) {
            // �w����Ԃ��߂����珜�O
            it->second -= (tickCount - m_tick_count);
            if (it->second > 0) {
                return true;
            } else {
                m_emergency_tactics.erase(it);
            }
        }

        return false;
    }

    /*!
     *  @brief  ������ߓo�^
     *  @param  tactics_id  �헪�ŗLID
     *  @param  command     ����
     */
    void EntryCommand(int32_t tactics_id, const StockTradingCommand& command)
    {
        const uint32_t code = command.GetCode();

        // ���ߎ�ʂ��Ƃ̏���
        switch (command.GetType())
        {
        case StockTradingCommand::EMERGENCY:
            // �Ώې헪ID�ɑ����閽�߂͎̂Ă�
            std::remove_if(m_command_list.begin(),
                           m_command_list.end(),
                           [tactics_id, code](std::pair<int32_t, StockTradingCommand>& elem) {
                return (tactics_id == elem.first &&
                        code == elem.second.GetCode() &&
                        !elem.second.IsForEmergencyCommand()); // �ً}�����߂͏��O
            });
            // �֎~���[�h�Ώێ҂Ƃ���
            m_emergency_tactics[tactics_id] = m_emergency_time_ms;
            // �擪�ɐς�
            m_command_list.emplace_front(tactics_id, command);
            break;

        case StockTradingCommand::ORDER:
            // ����헪/����O���[�v��ORDER������Ώ���
            // ����������Ă��܂��Ă���D��x�ɂ��������Ēe���������������߂�(>ToDo<)
            for (const auto& sv_order: m_server_order) {
                if (sv_order.second.first.m_tactics_id == tactics_id &&
                    sv_order.second.first.m_group_id == command.GetGroupID()) {
                    return; // �e��
                }
            }
            // �������ʑ҂����ĂĂ��e��(�������������߂�) >ToDo<
            if (!m_wait_order.empty()) {
                const auto& w_order = m_wait_order.top();
                if (w_order.first.m_tactics_id == tactics_id &&
                    w_order.first.m_group_id == command.GetGroupID()) {
                    return; // �e��
                }
            }
            for (const auto& lcommand: m_command_list) {
                if (lcommand.second.GetType() == StockTradingCommand::ORDER) {
                    if (lcommand.second.GetGroupID() == command.GetGroupID() &&
                        lcommand.second.GetTacticsID() == command.GetTacticsID()) {
                        return; // �e��
                    }
                }
            }
            m_command_list.emplace_back(tactics_id, command);
            break;

        default:
            // �����ɐς�
            m_command_list.emplace_back(tactics_id, command);
            break;
        }
    }

    /*!
     *  @brief  ���߂���������
     *  @param  aes_pwd
     */
    void IssueOrder(const CipherAES& aes_pwd)
    {
        if (m_command_list.empty()) {
            return; // ��
        }
        if (!m_wait_order.empty()) {
            return; // �҂�������
        }
        auto it = m_command_list.begin();
        const StockTradingCommand& command(it->second);
        const StockOrderIdentifier soidx(command.GetTacticsID(), command.GetGroupID());
        StockOrder order;
        order.m_investiments = m_investments;
        if (command.GetType() == StockTradingCommand::ORDER) {
            command.GetOrder(order);
            if (order.m_type == ORDER_BUY || order.m_type == ORDER_SELL) {
                m_wait_order.push(std::pair<StockOrderIdentifier, StockOrder>(soidx, order));
                std::wstring pwd;
                aes_pwd.Decrypt(pwd);
                m_pSession->FreshOrder(order, pwd, [this](bool b_result, const RcvResponseStockOrder& rcv_order) {
                    StockOrderCallback(b_result, rcv_order);
                });
            }
        }
        m_command_list.pop_front();
    }
};

/*!
 *  @param  session     �،���ЂƂ̃Z�b�V����
 *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
 */
StockOrderingManager::StockOrderingManager(const std::shared_ptr<SecuritiesSession>& session,
                                           const TradeAssistantSetting& script_mng)
: m_pImpl(new PIMPL(session, script_mng))
{
}
/*!
 */
StockOrderingManager::~StockOrderingManager()
{
}

/*!
 *  @brief  ����헪����
 *  @param  tickCount   �o�ߎ���[�~���b]
 *  @param  now_tm      ���ݎ����b
 *  @param  tactics     �헪�f�[�^
 *  @param  valuedata   ���i�f�[�^(1�������)
 *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
 */
void StockOrderingManager::InterpretTactics(int64_t tickCount,
                                            const HHMMSS& hhmmss,
                                            eStockInvestmentsType investments,
                                            const std::vector<StockTradingTactics>& tactics,
                                            const std::vector<StockPortfolio>& valuedata,
                                            TradeAssistantSetting& script_mng)
{
    // >ToDo< �������ʂ��ς�����獡���閽�߃��X�getc��j��

    // ����
    for (const auto& tac: tactics) {
        const bool b_emergency = m_pImpl->Update_Emergency(tickCount, tac);

        tac.Interpret(b_emergency,
                      hhmmss,
                      valuedata,
                      script_mng,
                      [this](int32_t tactics_id, const StockTradingCommand& command) {
            m_pImpl->EntryCommand(tactics_id, command);
        });
    }

    m_pImpl->Correct(tickCount, investments);
}

/*!
 *  @brief  ���߂���������
 *  @param  aes_pwd
 */
void StockOrderingManager::IssueOrder(const CipherAES& aes_pwd)
{
    m_pImpl->IssueOrder(aes_pwd);
}

} // namespace trading
