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
 *  @brief  �R�}���h����(�ً}���[�h)
 *  @param  code            �����R�[�h
 *  @param  tactics_id      �헪ID
 *  @param  target_group    �ΏۃO���[�v<�헪�O���[�vID>
 */
StockTradingCommand::StockTradingCommand(const StockCode& code,
                                         int32_t tactics_id,
                                         const std::unordered_set<int32_t>& target_group)
: m_type(StockTradingCommand::EMERGENCY)
, m_code(code.GetCode())
, m_tactics_id(tactics_id)
, iparam()
, fparam(0.0)
{
    iparam.reserve(target_group.size());
    for (int32_t gid: target_group) {
        iparam.push_back(gid);
    }
}

/*!
 *  @brief  �R�}���h�쐬(����)
 *  @param  code        �����R�[�h
 *  @param  tactics_id  �헪ID
 *  @param  group_id    �헪�O���[�vID
 *  @param  unique_id   �헪�����ŗLID
 *  @param  order_type  �������(eOrderType)
 *  @param  order_cond  ��������(eOrderCondition)
 *  @param  b_leverage  �M�p����t���O
 *  @param  number      ��������
 *  @param  value       �������i
 */
StockTradingCommand::StockTradingCommand(const StockCode& code,
                                        int32_t tactics_id,
                                        int32_t group_id,
                                        int32_t unique_id,
                                        int32_t order_type,
                                        int32_t order_cond,
                                        bool b_leverage,
                                        int32_t number,
                                        float64 value)
: m_type(StockTradingCommand::ORDER)
, m_code(code.GetCode())
, m_tactics_id(tactics_id)
, iparam()
, fparam(value)
{
    iparam.reserve(NUM_IPARAM_ORDER);
    iparam.push_back(group_id);
    iparam.push_back(unique_id);
    iparam.push_back(order_type);
    iparam.push_back(order_cond);
    iparam.push_back(static_cast<int32_t>(b_leverage));
    iparam.push_back(number);
    iparam.push_back(-1);   // �����ԍ�������
}

/*!
 *  @brief  �R�}���h�쐬(����[����])
 *  @param  src_command �㏑�����锄������
 *  @param  order_type  �������
 *  @param  order_id    �����ԍ�(�،���Ђ����s��������)
 */
StockTradingCommand::StockTradingCommand(const StockTradingCommand& src_command,
                                         int32_t order_type,
                                         int32_t order_id)
: m_type(StockTradingCommand::ORDER)
, m_code(src_command.GetCode())
, m_tactics_id(src_command.GetTacticsID())
, iparam(src_command.iparam)
, fparam(src_command.fparam)
{
    if (src_command.iparam.size() == NUM_IPARAM_ORDER) {
        iparam[IPARAM_ORDER_TYPE] = order_type;
        iparam[IPARAM_ORDER_ID] = order_id;
    } else {
        m_type = StockTradingCommand::NONE; // error
    }
}

/*!
 *  @brief  �R�}���h�쐬(����[���])
 *  @param  src_order   ��������������
 *  @param  tactics_id  �헪ID
 *  @param  group_id    �헪�O���[�vID
 *  @param  unique_id   �헪�����ŗLID
 *  @param  order_type  �������(eOrderType)
 */
StockTradingCommand::StockTradingCommand(const StockOrder& src_order,
                                         int32_t tactics_id,
                                         int32_t group_id,
                                         int32_t unique_id,
                                         int32_t order_type,
                                         int32_t order_id)
: m_type(StockTradingCommand::ORDER)
, m_code(src_order.m_code.GetCode())
, m_tactics_id(tactics_id)
, iparam()
, fparam(src_order.m_value)
{
    iparam.reserve(NUM_IPARAM_ORDER);
    iparam.push_back(group_id);
    iparam.push_back(unique_id);
    iparam.push_back(order_type);
    iparam.push_back(static_cast<int32_t>(src_order.m_condition));
    iparam.push_back(static_cast<int32_t>(src_order.m_b_leverage));
    iparam.push_back(src_order.m_number);
    iparam.push_back(order_id);
}

/*!
 *  @brief  �����t���[�p�����[�^���}�[�W����
 *  @param  src
 */
void StockTradingCommand::MergeIntFreeparam(const std::vector<int32_t>& src)
{
    if (!iparam.empty()) {
        iparam = src;
    } else {
        for (int32_t si: src) {
            if (iparam.end() == std::find(iparam.begin(), iparam.end(), si)) {
                iparam.push_back(si);
            }
        }
    }
}
    
/*!
 *  @brief  �������p�����[�^�𓾂�
 *  @param[out] dst �i�[��
 */
void StockTradingCommand::GetOrder(StockOrder& dst) const
{
    if (m_type == ORDER && iparam.size() == static_cast<size_t>(NUM_IPARAM_ORDER)) {
        dst.m_code = m_code;
        dst.m_number = iparam[IPARAM_NUMBER];
        dst.m_value = fparam;
        dst.m_b_market_order = static_cast<int32_t>(fparam) <= 0;
        dst.m_b_leverage = (iparam[IPARAM_B_LEVERAGE] != 0);

        dst.m_type = static_cast<eOrderType>(iparam[IPARAM_ORDER_TYPE]);
        dst.m_condition = static_cast<eOrderConditon>(iparam[IPARAM_ORDER_CONDITION]);
    }
}

/*!
 *  @brief  right�͏�ʂ̔����������H
 *  @param  right   ��r���閽��
 */
bool StockTradingCommand::IsUpperBuySellOrder(const StockTradingCommand& right) const
{
    if (GetType() == ORDER && right.GetType() == ORDER) {
        const eOrderType order_type = static_cast<eOrderType>(GetOrderType());
        const eOrderType r_order_type = static_cast<eOrderType>(right.GetOrderType());
        if (order_type == r_order_type && (order_type == ORDER_BUY || order_type == ORDER_SELL)) {
            return right.GetOrderGroupID() == GetOrderGroupID() &&
                   right.GetTacticsID() == GetTacticsID() &&
                   right.GetCode() == GetCode() &&
                   right.GetOrderUniqueID() > GetOrderUniqueID();
        }
    }
    return false;    
}

} // namespace trading
