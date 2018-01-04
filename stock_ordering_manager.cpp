/*!
 *  @file   stock_ordering_manager.cpp
 *  @brief  �������Ǘ���
 *  @date   2017/12/26
 */
#include "stock_ordering_manager.h"

#include "securities_session.h"
#include "stock_portfolio.h"
#include "stock_trading_command.h"
#include "stock_trading_tactics.h"
#include "trade_assistant_setting.h"
#include "trade_struct.h"

#include "cipher_aes.h"
#include "twitter_session.h"
#include "update_message.h"
#include "utility_datetime.h"
#include "utility_string.h"

#include <list>

namespace trading
{
class StockOrderingManager::PIMPL
{
private:
    /*!
     *  @brief  �헪���ʏ��
     *  @note   a��b����v����Γ����Ac����v�����瓯��̒���
     */
    struct TacticsIdentifier
    {
        int32_t m_tactics_id;   //!< �헪ID...(a)
        int32_t m_group_id;     //!< �헪�O���[�vID...(b)
        int32_t m_unique_id;    //!< �헪�����ŗLID...(c)

        TacticsIdentifier(const StockTradingCommand& command)
        : m_tactics_id(command.GetTacticsID())
        , m_group_id(command.GetOrderGroupID())
        , m_unique_id(command.GetOrderUniqueID())
        {
        }
    };
    /*!
     *  @brief  �ً}���[�h���
     */
    struct EmergencyModeState
    {
        uint32_t m_code;
        int32_t m_tactics_id;
        std::unordered_set<int32_t> m_group;

        int64_t m_timer; //! �c�莞��

        EmergencyModeState(uint32_t code, int32_t tactics_id, const std::vector<int32_t>& group, int64_t timer)
        : m_code(code)
        , m_tactics_id(tactics_id)
        , m_group()
        , m_timer(timer)
        {
            AddGroupID(group);
        }

        void AddGroupID(const std::vector<int32_t>& group)
        {
            for (int32_t group_id: group) {
                m_group.insert(group_id);
            }
        }
    };

    //! �،���ЂƂ̃Z�b�V����
    std::shared_ptr<SecuritiesSession> m_pSecSession;
    //! twitter�Ƃ̃Z�b�V����
    std::shared_ptr<TwitterSessionForAuthor> m_pTwSession;

    //! ����헪�f�[�^<�헪ID, �헪�f�[�^>
    std::unordered_map<int32_t, StockTradingTactics> m_tactics;
    //! �헪�f�[�^�R�t�����<�����R�[�h, �헪ID>
    std::vector<std::pair<uint32_t, int32_t>> m_tactics_link;
    //! �ً}���[�h����[�~���b] ���O���ݒ肩��擾
    const int64_t m_emergency_time_ms;
    //! ������<�����R�[�h, ���O(utf-16)>
    std::unordered_map<uint32_t, std::wstring> m_stock_name;
    //! �Ď������f�[�^<��������, <�����R�[�h, 1�������̊Ď������f�[�^>>
    std::unordered_map<eStockInvestmentsType, std::unordered_map<uint32_t, StockPortfolio>> m_portfolio;

    //! ���߃��X�g
    std::list<StockTradingCommand> m_command_list;
    //! �ً}���[�h���
    std::list<EmergencyModeState> m_emergency_state;
    //! ���ʑ҂�����<���ʏ��, �����p�����[�^> ���v�f����1��0/������1����������
    std::vector<std::pair<TacticsIdentifier, StockOrder>> m_wait_order;
    //! ������<��������, <�Ǘ��p�����ԍ�, <���ʏ��, �����p�����[�^>>>
    std::unordered_map<eStockInvestmentsType, std::unordered_map<int32_t, std::pair<TacticsIdentifier, StockOrder>>> m_server_order;
    //! �����ԍ��Ή��\<�\���p�����ԍ�, �Ǘ��p�����ԍ�>
    std::unordered_map<int32_t, int32_t> m_server_order_id;

    //! ���݂̑Ώێ����
    eStockInvestmentsType m_investments;
    //! �o�ߎ���[�~���b]
    int64_t m_tick_count;

    /*!
     *  @brief  ���߂ƒ����̑�����r
     *  @param  left    ����
     *  @param  right   ����+���ʏ��
     */
    bool static CompareOrderAttr(const StockTradingCommand& left, const std::pair<TacticsIdentifier, StockOrder>& right)
    {
        // ����E���헪�̒����Ȃ��true
        return left.GetType() == StockTradingCommand::ORDER &&
               left.GetOrderType() == static_cast<int>(right.second.m_type) &&
               left.GetCode() == right.second.m_code.GetCode() &&
               left.GetTacticsID() == right.first.m_tactics_id &&
               left.GetOrderGroupID() == right.first.m_group_id;
    }

    void AddErrorMsg(const std::wstring& err, std::wstring& dst)
    {
        if (dst.empty()) {
            dst = err;
        } else {
            dst += twitter::GetNewlineString() + err;
        }
    }

    /*!
     *  @brief  �����������ʃ��b�Z�[�W�o��
     *  @param  b_result    ����
     *  @param  order       �����p�����[�^
     *  @param  order_id    �����ԍ�
     *  @param  name        ������
     *  @param  err         �G���[���b�Z�[�W
     *  @param  date        ��������
     *  @note   �o�͐��twitter
     */
    void OutputMessage(bool b_result,
                       const StockOrder& order,
                       int32_t order_id,
                       const std::wstring& err,
                       const std::wstring& date)
    {
        const std::wstring nl(std::move(twitter::GetNewlineString()));
        std::wstring src((b_result) ?L"������t" : L"�������s");
        if (order.m_type == ORDER_NONE) {
            // order����ŌĂ΂�邱�Ƃ�����(�������s��)
        } else {
            const uint32_t code = order.m_code.GetCode();
            switch (order.m_type)
            {
            case ORDER_BUY:
                if (order.m_b_leverage) {
                    src += L"(�M�p�V�K��)";
                } else {
                    src += L"(������)";
                }
                break;
            case ORDER_SELL:
                src += L"(�M�p�V�K��)";
                break;
            case ORDER_CORRECT:
                src += L"(��������)";
                break;
            case ORDER_CANCEL:
                src += L"(�������)";
                break;
            }
            src += L" " + utility::ToSecuretIDOrder(order_id, 4);
            src += nl + std::to_wstring(code) + L" " + m_stock_name[code];
            src += nl + L"���� " + std::to_wstring(order.m_number);
            src += nl + L"���i " + utility::ToWstringOrder(order.m_value, 1);
            if (order.m_b_market_order) {
                src += L"(���s)";
            }
        }
        if (!err.empty()) {
            src += nl + L"[error]" + nl + err;
        }
        m_pTwSession->Tweet(date, src);
    }

    /*!
     *  @brief  �����R�[���o�b�N
     *  @param  b_result    ����
     *  @param  rcv_order   ��������
     *  @param  sv_date     �T�[�o����
     *  @param  investments �������̎�������
     *  @note   �ʐM�x���Ōׂ��\��������̂Ō���(this)��investments�͎g��Ȃ�
     */
    void StockOrderCallback(bool b_result,
                            const RcvResponseStockOrder& rcv_order,
                            const std::wstring& sv_date,
                            eStockInvestmentsType investments)
    {
        if (m_wait_order.empty()) {
            // �Ȃ��������҂����Ȃ�(error)
            const std::wstring err_msg(L"%wait_order is empty");
            OutputMessage(b_result, StockOrder(rcv_order), rcv_order.m_order_id, err_msg, sv_date);
        } else {
            const auto& w_order = m_wait_order.front();
            const TacticsIdentifier& tc_idx = w_order.first;
            const StockOrder& order = w_order.second;
            //
            std::wstring err_msg;
            if (b_result) {
                const float64 diff_value = order.m_value - rcv_order.m_value;
                if (order.m_code.GetCode() == rcv_order.m_code &&
                    order.m_type == rcv_order.m_type &&
                    order.m_investiments == rcv_order.m_investments &&
                    ((order.m_b_leverage && rcv_order.m_b_leverage) || (!order.m_b_leverage && !rcv_order.m_b_leverage)) &&
                    (-0.05 < diff_value && diff_value < 0.05) && /* �덷0.05�����͋��e(���������_�̈�v��r�͂�����̂�) */
                    order.m_number == rcv_order.m_number) {
                } else {
                    // ��t�Ƒ҂����H������Ă�(error)
                    err_msg = L"isn't equal %wait_order and %rcv_order";
                }
                switch (order.m_type)
                {
                case ORDER_BUY:
                case ORDER_SELL:
                    m_server_order[investments].emplace(rcv_order.m_order_id, std::pair<TacticsIdentifier, StockOrder>(tc_idx, order));
                    m_server_order_id.emplace(rcv_order.m_user_order_id, rcv_order.m_order_id);
                    break;
                case ORDER_CANCEL:
                    {
                        auto itID = m_server_order_id.find(rcv_order.m_user_order_id);
                        if (itID != m_server_order_id.end()) {
                            if (0 == m_server_order[investments].erase(itID->second)) {
                                AddErrorMsg(L"fail to erase %server_order", err_msg);
                            }
                        } else {
                            AddErrorMsg(L"fail to cnv %order_id from %user_order_id", err_msg);
                        }
                    }
                    break;
                default:
                    break;
                }
            }
            // �ʒm(m_wait_order���Q�Ƃ��Ă�̂Ł����ŏ������ʂ�Ă�)
            OutputMessage(b_result, order, rcv_order.m_user_order_id, err_msg, sv_date);
            // �󂯕t����ꂽ�璍���҂�����
            if (b_result) {
                m_wait_order.pop_back();
            }
        }
    }

    /*!
     *  @brief  �ً}���[�h��ԓo�^
     *  @param  command     ����
     */
    void EntryEmergencyState(const StockTradingCommand& command)
    {
        const uint32_t code = command.GetCode();
        const int32_t tactics_id = command.GetTacticsID();
        const std::vector<int32_t>& group = command.RefEmergencyTargetGroup();
        for (auto& emstat: m_emergency_state) {
            if (emstat.m_code == code && emstat.m_tactics_id == tactics_id) {
                // ���łɂ���΍X�V
                emstat.AddGroupID(group);
                emstat.m_timer = m_emergency_time_ms;
                return;
            }
        }
        m_emergency_state.emplace_back(code, tactics_id, group, m_emergency_time_ms);
    }

    /*!
     *  @brief  ������ߓo�^
     *  @param  command     ����
     */
    void EntryCommand(const StockTradingCommand& command)
    {
        const uint32_t code = command.GetCode();
        const int32_t tactics_id = command.GetTacticsID();

        // ���ߎ�ʂ��Ƃ̏���
        switch (command.GetType())
        {
        case StockTradingCommand::EMERGENCY:
            // �ً}���[�h��ԓo�^
            EntryEmergencyState(command);
            // �����O�����폜
            {
                auto itRmv = std::remove_if(m_command_list.begin(),
                                            m_command_list.end(),
                                            [&command, this](StockTradingCommand& dst) {
                    if (dst.GetType() == StockTradingCommand::ORDER &&
                        command.GetCode() == dst.GetCode() &&
                        command.GetTacticsID() == dst.GetTacticsID()) {
                        const eOrderType od_type = static_cast<eOrderType>(dst.GetOrderType());
                        if (od_type == ORDER_BUY || od_type == ORDER_SELL || od_type == ORDER_CORRECT) {
                            const int32_t group_id = dst.GetOrderGroupID();
                            for (const int32_t em_gid: command.RefEmergencyTargetGroup()) {
                                if (group_id == em_gid) {
                                    return true;
                                }
                            }
                            return false;
                        }
                    }
                    return false;
                });
                if (m_command_list.end() != itRmv) {
                    m_command_list.erase(itRmv, m_command_list.end());
                }
            }
            // �����ςݒ������
            for (const auto& sv_order: m_server_order[m_investments]) {
                const TacticsIdentifier& tc_idx(sv_order.second.first);
                const StockOrder& order(sv_order.second.second);
                if (tc_idx.m_tactics_id == tactics_id && order.m_code.GetCode() == code) {
                    const int32_t order_id = sv_order.first;
                    auto itCr = std::find_if(m_command_list.begin(), m_command_list.end(),
                                             [order_id](const StockTradingCommand& com) {
                        return com.GetOrderType() == ORDER_CANCEL && com.GetOrderID() == order_id;
                    });
                    if (itCr != m_command_list.end()) {
                        continue; // �����ς�ł���
                    }
                    const auto& em_group(command.RefEmergencyTargetGroup());
                    auto itEm = std::find(em_group.begin(), em_group.end(), tc_idx.m_group_id);
                    if (itEm != em_group.end()) {
                        // ���������擪�ɐς�
                        StockTradingCommand cancel_command(order,
                                                           tactics_id,
                                                           tc_idx.m_group_id,
                                                           tc_idx.m_unique_id,
                                                           static_cast<int32_t>(ORDER_CANCEL),
                                                           order_id);
                        m_command_list.emplace_front(cancel_command);
                    }
                }
            }
            break;

        case StockTradingCommand::ORDER:
            // �������ʑ҂������`�F�b�N
            if (!m_wait_order.empty()) {
                const auto& w_order = m_wait_order.front();
                if (PIMPL::CompareOrderAttr(command, w_order)) {
                    // �����������҂����Ă�̂Œe��(����������ɑΏ�����)
                    return;
                }
            }
            // �����ςݒ����`�F�b�N
            for (const auto& sv_order: m_server_order[m_investments]) {
                if (PIMPL::CompareOrderAttr(command, sv_order.second)) {
                    if (sv_order.second.first.m_unique_id >= command.GetOrderUniqueID()) {
                        // ����A�܂��͓�����ʒ��������ς݂Ȃ̂Ŗ���
                        return;
                    }
                    // �������ʖ��ߔ����ς݂̂��߉��i�����𖖔��ɐς�
                    StockTradingCommand correct_command(command, static_cast<int32_t>(ORDER_CORRECT), sv_order.first);
                    m_command_list.emplace_back(correct_command);
                    return;
                }
            }
            // �����ҋ@�����`�F�b�N
            for (auto& lcommand: m_command_list) {
                if (lcommand.IsUpperBuySellOrder(command)) {
                    // �㏑��(�㏟��)
                    lcommand = command;
                    return;
                }
            }
            // �����ɐς�
            m_command_list.emplace_back(command);
            break;

        default:
            break;
        }
    }


    /*!
     *  @brief  �헪����
     *  @param  hhmmss      ���ݎ����b
     *  @param  valuedata   ���i�f�[�^(1�������)
     *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
     */
    void InterpretTactics(const HHMMSS& hhmmss,
                          const std::unordered_map<uint32_t, StockPortfolio>& valuedata,
                          TradeAssistantSetting& script_mng)
    {
        std::unordered_set<int32_t> blank_group;
        for (const auto& link: m_tactics_link) {
            const uint32_t code = link.first;
            const int32_t tactics_id = link.second;
            //
            const auto itVData = valuedata.find(code);
            if (itVData == valuedata.end()) {
                continue; // ���i�f�[�^���܂��Ȃ�
            }
            //
            const auto itEmStat = std::find_if(m_emergency_state.begin(),
                                               m_emergency_state.end(),
                                               [code, tactics_id](const EmergencyModeState& emstat)
            {
                return emstat.m_code == code && emstat.m_tactics_id == tactics_id;
            });
            //
            const auto& r_group = (itEmStat != m_emergency_state.end()) ?itEmStat->m_group :blank_group;
            m_tactics[tactics_id].Interpret(hhmmss,
                                            r_group,
                                            itVData->second,
                                            script_mng,
                                            [this](const StockTradingCommand& command) { EntryCommand(command); });
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
        const StockTradingCommand& command(m_command_list.front());
        if (command.GetType() != StockTradingCommand::ORDER) {
            // �s���Ȗ��߂��ς܂�Ă��������ret(error)
            m_command_list.pop_front();
            return;
        }

        // ���ʑ҂������ɐς�
        StockOrder order;
        command.GetOrder(order);
        const eStockInvestmentsType investments = m_investments;
        order.m_investiments = investments;
        m_wait_order.emplace_back(std::pair<TacticsIdentifier, StockOrder>(command, order));
        //
        std::wstring pwd;
        aes_pwd.Decrypt(pwd);
        const auto callabck = [this, investments](bool b_result,
                                                  const RcvResponseStockOrder& rcv_order,
                                                  const std::wstring& sv_date) {
            StockOrderCallback(b_result, rcv_order, sv_date, investments);
        };
        switch (order.m_type)
        {
        case ORDER_BUY:
        case ORDER_SELL:
            m_pSecSession->FreshOrder(order, pwd, callabck);
            break;
        case ORDER_CORRECT:
            m_pSecSession->CorrectOrder(command.GetOrderID(), order, pwd, callabck);
            break;
        case ORDER_CANCEL:
            m_pSecSession->CancelOrder(command.GetOrderID(), pwd, callabck);
            break;
        default:
            // �s���Ȗ��߂��ς܂�Ă���A���ʑ҂��������(error)
            m_wait_order.pop_back();
            break;
        }
        //
        m_command_list.pop_front();
    }

public:
    /*!
     *  @param  sec_session �،���ЂƂ̃Z�b�V����
     *  @param  tw_session  twitter�Ƃ̃Z�b�V����
     *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
     */
    PIMPL(const std::shared_ptr<SecuritiesSession>& sec_session,
          const std::shared_ptr<TwitterSessionForAuthor>& tw_session,
          TradeAssistantSetting& script_mng)
    : m_pSecSession(sec_session)
    , m_pTwSession(tw_session)
    , m_tactics()
    , m_tactics_link()
    , m_emergency_time_ms(utility::ToMiliSecondsFromSecond(script_mng.GetEmergencyCoolSecond()))
    , m_stock_name()
    , m_portfolio()
    , m_command_list()
    , m_emergency_state()
    , m_wait_order()
    , m_server_order()
    , m_server_order_id()
    , m_investments(INVESTMENTS_NONE)
    , m_tick_count(0)
    {
        UpdateMessage msg;
        if (!script_mng.BuildStockTactics(msg, m_tactics, m_tactics_link)) {
            // ���s
        }
    }


    /*!
     *  @brief  �Ď������R�[�h�擾
     *  @param[out] dst �i�[��
     */
    void GetMonitoringCode(std::unordered_set<uint32_t>& dst)
    {
        for (const auto& link: m_tactics_link) {
            dst.insert(link.first);
        }
    }

    /*!
     *  @brief  �|�[�g�t�H���I������
     *  @param  investments_type    ��������
     *  @param  rcv_portfolio       ��M�����|�[�g�t�H���I(�Ȉ�)
     *  @retval true                ����
     */
    bool InitPortfolio(eStockInvestmentsType investments_type,
                       const std::unordered_map<uint32_t, std::wstring>& rcv_portfolio)
    {
        // ��M�����|�[�g�t�H���I���Ď������ƈ�v���Ă邩�`�F�b�N
        std::unordered_map<uint32_t, StockPortfolio> portfolio;
        std::unordered_set<uint32_t> monitoring_code;
        GetMonitoringCode(monitoring_code);
        portfolio.reserve(monitoring_code.size());
        for (uint32_t code: monitoring_code) {
            if (rcv_portfolio.end() != rcv_portfolio.find(code)) {
                portfolio.emplace(code, StockPortfolio(code));
            } else {
                return false;
            }
        }
        // �󂾂�����V�K�쐬
        auto portfolio_it = m_portfolio.find(investments_type);
        if (portfolio_it == m_portfolio.end()) {
            m_portfolio.emplace(investments_type, portfolio);
            m_stock_name = rcv_portfolio;
        }
        return true;
    }

    /*!
     *  @brief  ���i�f�[�^�X�V
     *  @param  investments_type    ��������
     *  @param  senddate            ���i�f�[�^���M����
     *  @param  rcv_valuedata       �󂯎�������i�f�[�^
     */
    void UpdateValueData(eStockInvestmentsType investments_type,
                         const std::wstring& sendtime,
                         const std::vector<RcvStockValueData>& rcv_valuedata)
    {
        auto itPf = m_portfolio.find(investments_type);
        if (itPf != m_portfolio.end()) {
            std::tm tm_send; // ���i�f�[�^���M����(�T�[�o�^�C��)
            auto pt(utility::ToLocalTimeFromRFC1123(sendtime));
            utility::ToTimeFromBoostPosixTime(pt, tm_send);
            auto& portfolio(itPf->second);
            for (const auto& vunit: rcv_valuedata) {
                auto it = portfolio.find(vunit.m_code);
                if (it != portfolio.end()) {
                    it->second.UpdateValueData(vunit, tm_send);
                } else {
                    // ������Ȃ�������ǂ�����H(error)
                }
            }
        } else {
            // �Ȃ�������������ĂȂ�(error)
        }
    }

    /*!
     *  @brief  ����X�V
     *  @param  tickCount   �o�ߎ���[�~���b]
     *  @param  hhmmss      ���ݎ����b
     *  @param  investments ��������
     *  @param  valuedata   ���i�f�[�^(1�������)
     *  @param  aes_pwd
     *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
     */
    void Update(int64_t tickCount,
                const HHMMSS& hhmmss,
                eStockInvestmentsType investments,
                const CipherAES& aes_pwd,
                TradeAssistantSetting& script_mng)
    {
        // �������ʂ��ς�����獡���閽�߃��X�getc��j��
        if (investments != m_investments) {
            m_command_list.clear();
            m_emergency_state.clear();
        }

        // �ً}���[�h��ԍX�V
        {
            const int64_t diff_time = tickCount - m_tick_count;
            auto itRmv = std::remove_if(m_emergency_state.begin(),
                                        m_emergency_state.end(),
                                        [diff_time](EmergencyModeState& emstat)
            {
                emstat.m_timer -= diff_time;
                return emstat.m_timer <= 0;
            });
            if (itRmv != m_emergency_state.end()) {
                m_emergency_state.erase(itRmv, m_emergency_state.end());
            }
        }
        // �헪����
        InterpretTactics(hhmmss, m_portfolio[investments], script_mng);
        // ���ߏ���
        IssueOrder(aes_pwd);
        //
        m_tick_count = tickCount;
        m_investments = investments;
    }

};

/*!
 *  @param  sec_session �،���ЂƂ̃Z�b�V����
 *  @param  tw_session  twitter�Ƃ̃Z�b�V����
 *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
 */
StockOrderingManager::StockOrderingManager(const std::shared_ptr<SecuritiesSession>& sec_session,
                                           const std::shared_ptr<TwitterSessionForAuthor>& tw_session,
                                           TradeAssistantSetting& script_mng)
: m_pImpl(new PIMPL(sec_session, tw_session, script_mng))
{
}
/*!
 */
StockOrderingManager::~StockOrderingManager()
{
}

/*!
 *  @brief  �Ď������R�[�h�擾
 *  @param[out] dst �i�[��
 */
void StockOrderingManager::GetMonitoringCode(std::unordered_set<uint32_t>& dst)
{
    m_pImpl->GetMonitoringCode(dst);
}

/*!
 *  @brief  �|�[�g�t�H���I������
 *  @param  investments_type    ��������
 *  @param  rcv_portfolio       ��M�����|�[�g�t�H���I<�����R�[�h�ԍ�, ������(utf-16)>
 */
bool StockOrderingManager::InitPortfolio(eStockInvestmentsType investments_type,
                                         const std::unordered_map<uint32_t, std::wstring>& rcv_portfolio)
{
    return m_pImpl->InitPortfolio(investments_type, rcv_portfolio);
}

/*!
 *  @brief  ���i�f�[�^�X�V
 *  @param  investments_type    ��������
 *  @param  senddate            ���i�f�[�^���M����
 *  @param  rcv_valuedata       �󂯎�������i�f�[�^
 */
void StockOrderingManager::UpdateValueData(eStockInvestmentsType investments_type,
                                           const std::wstring& sendtime,
                                           const std::vector<RcvStockValueData>& rcv_valuedata)
{
    m_pImpl->UpdateValueData(investments_type, sendtime, rcv_valuedata);
}

/*!
 *  @brief  ����X�V
 *  @param  tickCount   �o�ߎ���[�~���b]
 *  @param  hhmmss      ���ݎ����b
 *  @param  investments ��������
 *  @param  valuedata   ���i�f�[�^(1�������)
 *  @param  aes_pwd
 *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
 */
void StockOrderingManager::Update(int64_t tickCount,
                                  const HHMMSS& hhmmss,
                                  eStockInvestmentsType investments,
                                  const CipherAES& aes_pwd,
                                  TradeAssistantSetting& script_mng)
{
    m_pImpl->Update(tickCount, hhmmss, investments, aes_pwd, script_mng);
}


} // namespace trading
