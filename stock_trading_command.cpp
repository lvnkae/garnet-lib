/*!
 *  @file   stock_trading_command.h
 *  @brief  ���������
 *  @date   2017/12/27
 */
#pragma once

#include "stock_trading_command.h"
#include "stock_code.h"
#include "trade_struct.h"

namespace trading
{

/*!
 *  @param  type    ���ߎ��
 *  @param  code    �����R�[�h
 *  @param   tid     �헪ID
 *  @param  gid     �헪���O���[�vID
 */
StockTradingCommand::StockTradingCommand(StockTradingCommand::eType type, const StockCode& code, int32_t tid, int32_t gid)
: m_type(type)
, m_code(code.GetCode())
, m_tactics_id(tid)
, m_group_id(gid)
, param0(0)
, param1(0)
, param2(0)
, param3(0)
, param4(0)
, fparam0(0.0)
{
}

/*!
 *  @brief  �������p�����[�^�𓾂�
 *  @param[out] dst �i�[��
 */
void StockTradingCommand::GetOrder(StockOrder& dst) const
{
    if (m_type == ORDER) {
        dst.m_code = m_code;
        dst.m_number = param3;
        dst.m_value = fparam0;
        dst.m_b_market_order = static_cast<int32_t>(fparam0) <= 0;
        dst.m_b_leverage = (param4 != 0);

        dst.m_type = static_cast<eOrderType>(param1);
        dst.m_condition = static_cast<eOrderConditon>(param2);
    }
}

} // namespace trading
