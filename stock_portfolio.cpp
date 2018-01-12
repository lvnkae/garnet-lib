/*!
 *  @file   stock_portfolio.cpp
 *  @brief  ���Ď������f�[�^
 *  @date   2017/12/25
 */

#include "stock_portfolio.h"
#include "trade_utility.h"

#include "garnet_time.h"

namespace trading
{

/*!
 *  @brief  ���i�f�[�^�X�V
 *  @param  src     ���i�f�[�^
 *  @param  date    �擾����(�T�[�o���Ԃ��g��)
 */
void StockValueData::UpdateValueData(const RcvStockValueData& src, const garnet::sTime& date)
{
    m_open = src.m_open;
    m_high = src.m_high;
    m_low  = src.m_low;
    m_close = src.m_close;

    if (m_value_data.empty()) {
        // �󂾂�����Ȃ�ł��o�^
    } else {
        // ��łȂ���΁A�O����o�����������Ă����ꍇ�����o�^
        const stockValue& last = m_value_data.back();
        if (last.m_volume >= src.m_volume) {
            return;
        }
    }
    m_value_data.emplace_back(date, src.m_value, src.m_volume);
};

} // namespace trading
