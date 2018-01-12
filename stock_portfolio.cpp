/*!
 *  @file   stock_portfolio.cpp
 *  @brief  株監視銘柄データ
 *  @date   2017/12/25
 */

#include "stock_portfolio.h"
#include "trade_utility.h"

#include "garnet_time.h"

namespace trading
{

/*!
 *  @brief  価格データ更新
 *  @param  src     価格データ
 *  @param  date    取得時刻(サーバ時間を使う)
 */
void StockValueData::UpdateValueData(const RcvStockValueData& src, const garnet::sTime& date)
{
    m_open = src.m_open;
    m_high = src.m_high;
    m_low  = src.m_low;
    m_close = src.m_close;

    if (m_value_data.empty()) {
        // 空だったらなんでも登録
    } else {
        // 空でなければ、前回より出来高が増えていた場合だけ登録
        const stockValue& last = m_value_data.back();
        if (last.m_volume >= src.m_volume) {
            return;
        }
    }
    m_value_data.emplace_back(date, src.m_value, src.m_volume);
};

} // namespace trading
