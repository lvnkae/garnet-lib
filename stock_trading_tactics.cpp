/*!
 *  @file   stock_trading_tactics.cpp
 *  @brief  ������헪
 *  @date   2017/12/08
 */
#include "stock_trading_tactics.h"

#include "stock_portfolio.h"
#include "stock_trading_command.h"
#include "trade_assistant_setting.h"
#include "trade_define.h"

#include <algorithm>
#include <ctime>

namespace trading
{

/*!
 */
StockTradingTactics::StockTradingTactics()
: m_code()
, m_emergency()
, m_fresh()
, m_repayment()
{
}


/*!
 *  @brief  �����R�[�h�Q�𓾂�
 *  @param[out] dst �i�[��
 */
void StockTradingTactics::GetCode(std::vector<StockCode>& dst) const
{ 
    dst = m_code;
}
/*!
 *  @brief  �����R�[�h���Z�b�g����
 *  @param  code    �����R�[�h
 */
void StockTradingTactics::SetCode(uint32_t code)
{
    m_code.push_back(code);
}
/*!
 *  @brief  �ً}���[�h�g���K�[��ǉ�����
 *  @param  trigger �g���K�[
 */
void StockTradingTactics::AddEmergencyTrigger(const Trigger& trigger)
{
    m_emergency.emplace_back(trigger);
}
/*!
 *  @brief  �V�K������ǉ�
 *  @param  order   �����ݒ�
 */
void StockTradingTactics::AddFreshOrder(const Order& order)
{
    m_fresh.emplace_back(order);
}
/*!
 *  @brief  �ԍϒ�����ǉ�
 *  @param  order   �����ݒ�
 */
void StockTradingTactics::AddRepaymentOrder(const Order& order)
{
    m_repayment.emplace_back(order);
}



/*!
 *  @brief  ����
 *  @param  hhmmss      ���ݎ����b
 *  @param  valuedata   ���i�f�[�^(1������)
 *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
 */
bool StockTradingTactics::Trigger::Judge(const HHMMSS& hhmmss,
                                         const StockPortfolio& valuedata,
                                         TradeAssistantSetting& script_mng) const
{
    if (valuedata.m_value_data.empty()) {
        return false; // ���i�f�[�^���Ȃ������画�肵�Ȃ�
    }

    switch (m_type)
    {
    case VALUE_GAP:
        {
            if (static_cast<int32_t>(valuedata.m_close) <= 0) {
                // 0���Z�΍�(error)
                // ��ꏉ�������͑O���I�l�����݂��Ȃ�(���唼�̓G���[)
                return false;
            }
            float64 v_high = 0.f;   // ���ԍ��l
            float64 v_low = 0.f;    // ���Ԉ��l
            float64 v_open = 0.f;   // ���Ԏn�l
            // �w�����[����,�w��b�O]�̎n�l/���l/���l�𓾂�
            {
                const int32_t pastsec = hhmmss.GetPastSecond();
                const int32_t rangesec = m_signed_param;
                const size_t presz = valuedata.m_value_data.size();
                //auto rit_end = valuedata.m_value_data.rend();
                int32_t rinx = 0;
                for (auto rit = valuedata.m_value_data.rbegin(); rit != valuedata.m_value_data.rend(); ++rit) {
                    if ((pastsec - rit->m_hhmmss.GetPastSecond()) <= rangesec) {
                        // �o�����Ȃ�(=�f�[�^�擪)�Ȃ�O���I�l�����l�Ƃ���
                        const float64 v_now = (rit->m_number == 0) ?valuedata.m_close
                                                                   :rit->m_value;
                        v_high = std::max(v_high, v_now);
                        v_low  = (static_cast<int32_t>(v_low) == 0) ?v_now
                                                                    :std::min(v_low, v_now);
                        v_open = v_now;
                    }
                    rinx++;
                }
            }
            // ���Ԏn�l�����݂���ꍇ�̂݃M���b�v�𒲂ׂ�
            if (v_open > 0.f) {
                if (m_float_param > 0.f) {
                    const float64 rateU = ((v_high-v_open)/v_open)*100.f;
                    return rateU >= m_float_param; 
                } else {
                    const float64 rateD = ((v_low-v_open)/v_open)*100.f;
                    return rateD <= m_float_param;
                }
            }
        }
        break;

    case NO_CONTRACT:
        {
            const int32_t pastsec = hhmmss.GetPastSecond();
            const auto& latest = valuedata.m_value_data.back();
            return ((pastsec - latest.m_hhmmss.GetPastSecond()) >= m_signed_param);
        }
        break;

    case SCRIPT_FUNCTION:
        {
            const auto& latest = valuedata.m_value_data.back();
            // �o�����Ȃ��Ȃ画�肵�Ȃ�(�f�[�^�擪�̂ݎ��ԋL�^�p�ɑ��݂�����)
            if (latest.m_number > 0) {
                return script_mng.CallJudgeFunction(m_signed_param,
                                                    latest.m_value,
                                                    valuedata.m_high,
                                                    valuedata.m_low,
                                                    valuedata.m_close);
            }
        }
    }

    return false;
}

/*!
 *  @brief  �헪����(1������)
 *  @param  b_emergency     �ً}���[�h��
 *  @param  hhmmss          ���ݎ����b
 *  @param  valuedata       �����i�f�[�^(1������)
 *  @param  script_mng      �O���ݒ�(�X�N���v�g)�Ǘ���
 *  @param  enqueue_func    ���߂��L���[�ɓ����֐�
 */
void StockTradingTactics::InterpretAtCode(bool b_emergency,
                                          const HHMMSS& hhmmss,
                                          const StockPortfolio& valuedata,
                                          TradeAssistantSetting& script_mng,
                                          const EnqueueFunc& enqueue_func) const
{
    const StockCode& s_code(valuedata.m_code);

    struct dbgWork
    {
        uint32_t m_code;
        HHMMSS m_hhmmss;
        int32_t m_trigger_type;
        int32_t m_trigger_param;
        float64 m_value;

        dbgWork(uint32_t code, const HHMMSS& hhmmss)
        : m_code(code)
        , m_hhmmss(hhmmss.m_hour, hhmmss.m_minute, hhmmss.m_second)
        , m_trigger_type(0)
        , m_trigger_param(0)
        , m_value(0.f)
        {}
    };

    static std::vector<dbgWork> emg_db;
    static std::vector<dbgWork> fr_db;
    static std::vector<dbgWork> rp_db;

    // �ً}���[�h����
    for (const auto& emg: m_emergency) {
        if (!emg.Judge(hhmmss, valuedata, script_mng)) {
            continue;
        }
        StockTradingCommand emergency_command(StockTradingCommand::EMERGENCY,
                                              s_code,
                                              valuedata.m_name,
                                              m_unique_id,
                                              0); // EMERGENCY��GroupID�Ȃ�
        enqueue_func(m_unique_id, emergency_command);
        b_emergency = true;
        //
        dbgWork db(s_code.GetCode(), hhmmss);
        db.m_trigger_type = emg.Dbg_GetType();
        db.m_trigger_param = emg.Dbg_GetSignedParam();
        db.m_value = emg.Dbg_GetFloatParam();
        emg_db.emplace_back(db);
    }
    // �V�K��������
    for (const auto& order: m_fresh) {
        if (b_emergency && !order.IsForEmergency()) {
            continue;
        }
        if (!order.Judge(hhmmss, valuedata, script_mng)) {
            continue;
        }
        const auto& latest = valuedata.m_value_data.back();
        const float64 value = script_mng.CallGetValueFunction(order.GetValueFuncReference(),
                                                              latest.m_value,
                                                              valuedata.m_high,
                                                              valuedata.m_low,
                                                              valuedata.m_close);
        StockTradingCommand order_command(StockTradingCommand::ORDER,
                                          s_code,
                                          valuedata.m_name,
                                          m_unique_id,
                                          order.GetGroupID());
        {
            trading::eOrderType otype = order.GetType() == BUY ?ORDER_BUY :ORDER_SELL;
            order_command.SetOrderParam(static_cast<int32_t>(otype),
                                        static_cast<int32_t>(CONDITION_NONE), // >ToDo< �����Ή�
                                        static_cast<uint32_t>(order.GetVolume()),
                                        value,
                                        order.GetIsLeverage());
        }
        enqueue_func(m_unique_id, order_command);
        //
        dbgWork db(s_code.GetCode(), hhmmss);
        db.m_trigger_type = order.Dbg_RefTrigger().Dbg_GetType();
        db.m_trigger_param = order.Dbg_RefTrigger().Dbg_GetSignedParam();
        db.m_value = value;
        fr_db.emplace_back(db);
    }
    // �ԍϒ�������
    for (const auto& order: m_repayment) {
        if (b_emergency && !order.IsForEmergency()) {
            continue;
        }
        if (!order.Judge(hhmmss, valuedata, script_mng)) {
            continue;
        }
        const auto& latest = valuedata.m_value_data.back();
        const float64 value = script_mng.CallGetValueFunction(order.GetValueFuncReference(),
                                                              latest.m_value,
                                                              valuedata.m_high,
                                                              valuedata.m_low,
                                                              valuedata.m_close);
        StockTradingCommand order_command(StockTradingCommand::ORDER,
                                          s_code,
                                          valuedata.m_name,
                                          m_unique_id,
                                          order.GetGroupID());
        {
            trading::eOrderType otype = order.GetType() == BUY ?ORDER_BUY :ORDER_SELL;
            order_command.SetOrderParam(static_cast<int32_t>(otype),
                                        static_cast<int32_t>(CONDITION_NONE), // >ToDo< �����Ή�
                                        static_cast<uint32_t>(order.GetVolume()),
                                        value,
                                        order.GetIsLeverage());
        }
        enqueue_func(m_unique_id, order_command);
        //
        dbgWork db(s_code.GetCode(), hhmmss);
        db.m_trigger_type = order.Dbg_RefTrigger().Dbg_GetType();
        db.m_trigger_param = order.Dbg_RefTrigger().Dbg_GetSignedParam();
        db.m_value = value;
        rp_db.emplace_back(db);
    }
}

/*!
 *  @brief  �헪����
 *  @param  b_emergency     �ً}���[�h��
 *  @param  hhmmss          ���ݎ����b
 *  @param  valuedata       ���i�f�[�^(1�������)
 *  @param  script_mng      �O���ݒ�(�X�N���v�g)�Ǘ���
 *  @param  enqueue_func    ���߂��L���[�ɓ����֐�
 */
void StockTradingTactics::Interpret(bool b_emergency,
                                    const HHMMSS& hhmmss,
                                    const std::vector<StockPortfolio>& valuedata,
                                    TradeAssistantSetting& script_mng,
                                    const EnqueueFunc& enqueue_func) const
{
    for (const StockCode& code: m_code) {
        auto it = std::find_if(valuedata.begin(), valuedata.end(), [&code](const StockPortfolio& vdunit) {
            return (vdunit.m_code.GetCode() == code.GetCode());
        });
        if (it == valuedata.end()) {
            // �Ȃ���������Ȃ�����(error)
            continue;
        }
        if (it->m_value_data.empty()) {
            // �܂����i�f�[�^���Ȃ�(���񉿊i�f�[�^�擾�O)
            continue;
        }
        InterpretAtCode(b_emergency, hhmmss, *it, script_mng, enqueue_func);
    }
}


} // namespace trading
