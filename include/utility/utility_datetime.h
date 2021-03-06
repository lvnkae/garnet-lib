/*!
 *  @file   utility_datetime.h
 *  @brief  [common]日時関連Utility
 *  @date   2017/12/19
 *  @note   boost::date_time に依存している
 */
#pragma once

#include <string>
#include "boost/date_time/posix_time/posix_time.hpp"

namespace garnet
{
struct sTime;
struct YYMMDD;
struct HHMMSS;

namespace utility_datetime
{

/*!
 *  @brief  曜日
 *  @note   tm::wday形式(days since Sunday - [0,6])
 */
enum WeekOfDay
{
    SUNDAY = 0,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
};

/*!
 *  @brief  RFC1123形式の日時文字列からローカル日時を得る
 *  @param  rfc1123 RFC1123形式日時文字列
 *  @return boost時間インターフェース
 *  @note   "Fri, 03 Jan 2014 08:39:11 GMT"
 *  @note   httpヘッダに入ってる↑形式のやつ
 *  @note   ※ローカル=GMT+09(JST)に固定
 */
boost::posix_time::ptime ToLocalTimeFromRFC1123(const std::string& rfc1123);
boost::posix_time::ptime ToLocalTimeFromRFC1123(const std::wstring& rfc1123);
/*!
 *  @brief  RFC1123形式日次文字列に変換
 *  @param  date    年月日
 *  @param  time    時刻
 *  @param  zone    タイムゾーン文字列
 */
std::string ToRFC1123(const garnet::YYMMDD& date,
                      const garnet::HHMMSS& time,
                      const std::string& zone);
std::wstring ToRFC1123(const garnet::YYMMDD& date,
                       const garnet::HHMMSS& time,
                       const std::wstring& zone);

/*!
 *  @brief  指定フォーマットの日時文字列をtmに変換
 *  @param[in]  src     日時文字列
 *  @param[in]  format  ↑形式 (%H:%M:%S など)
 *  @param[out] o_tm    格納先
 *  @note   ローカル変換なし
 */
bool ToTimeFromString(const std::string& src, const std::string& format, garnet::sTime& o_tm);
/*!
 *  @brief  ptimeからtmに変換
 *  @param[in]  src     ptime
 *  @param[out] o_tm    格納先
 *  @param[in]  tm::tm_isdstは常にfalse(サマータイム考慮しない)
 */
void ToTimeFromBoostPosixTime(const boost::posix_time::ptime& src, garnet::sTime& o_tm);

/*!
 *  @brief  経過時間を得る
 *  @return tickCount(ミリ秒)
 *  @note   WiinbaseのGetTickCountを使わない汎用処理
 *  @note   起点がどこなのかは不明(普通はシステム起動)
 */
int64_t GetTickCountGeneral();

/*!
 *  @brief  srcとシステムローカル時間との差を秒[絶対値]で得る
 *  @param  src 比べる時間
 *  @return 秒差
 */
uint32_t GetDiffSecondsFromLocalMachineTime(const garnet::sTime& src);
/*!
 *  @brief  システムローカル時間を文字列で得る
 *  @param[in]  format  取得形式 (%H:%M:%S など)
 *  @return 文字列
 */
std::wstring GetLocalMachineTime(const std::wstring& format);

/*!
 *  @brief  rightとleftの差を秒数(絶対値)で得る
 *  @param  right
 *  @param  left
 */
std::time_t DiffTimeBySecond(const garnet::sTime& right, const garnet::sTime& left);
/*!
 *  @brief  rightとleftの差を日数(絶対値)で得る
 *  @param  right
 *  @param  left
 *  @note   日未満の差(時分秒)は切り捨て
 */
int32_t DiffTimeByDay(const garnet::sTime& right, const garnet::sTime& left);

/*!
 *  @brief  年月日時分秒に差分[ミリ秒]を足す
 *  @param[in]  base_tm ベース
 *  @param[in]  diff_ms 差分[ミリ秒]
 *  @param[out] o_now   格納先
 */
void AddTimeAndDiffMS(const garnet::sTime& base_tm, int64_t diff_ms, garnet::sTime& o_now);

/*!
 *  @brief  起点日時からafter_day後の00:00までの時間をミリ秒で得る
 *  @param  base_tm     起点日時
 *  @param  after_day   何日後か(翌日が1)
 *  @return 残り時間[ミリ秒]
 *  @note   0日後指定なら常に0を返す
 */
int64_t GetAfterDayLimitMS(const garnet::sTime& base_tm, int32_t after_day);
/*!
 *  @brief  分をミリ秒で得る
 *  @param  minute  任意時間[分]
 *  @retval 任意時間[ミリ秒]
 */
int64_t ToMiliSecondsFromMinute(int32_t minute);
/*!
 *  @brief  秒をミリ秒で得る
 *  @param  second  任意時間[秒]
 *  @retval 任意時間[ミリ秒]
 */
int64_t ToMiliSecondsFromSecond(int32_t second);

/*!
 *  @brief  1時間の秒数
 */
int32_t SecondsOf1Hour();
/*!
 *  @brief  1分の秒数
 */
int32_t SecondsOf1Minute();
/*!
 *  @brief  月先頭日
 */
int32_t GetFirstDayOfMonth();
/*!
 *  @brief  1ヶ月の最大日数
 */
int32_t GetMaxDayOfMonth();


} // namespace utility_datetime
} // namespace garnet
