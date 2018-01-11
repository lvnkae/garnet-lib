/*!
 *  @file   stock_portfolio.cpp
 *  @brief  ���Ď������f�[�^
 *  @date   2017/12/25
 */

#include "stock_portfolio.h"
#include "trade_utility.h"

#include <ctime>

namespace trading
{

/*!
 *  @brief  ���i�f�[�^�X�V
 *  @param  src     ���i�f�[�^
 *  @param  date    �擾����(�T�[�o���Ԃ��g��)
 */
void StockValueData::UpdateValueData(const RcvStockValueData& src, const std::tm& date)
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
    stockValue vdata(date.tm_hour, date.tm_min, date.tm_sec);
    vdata.m_value  = src.m_value;
    vdata.m_volume = src.m_volume;

    m_value_data.push_back(vdata);
};

} // namespace trading
