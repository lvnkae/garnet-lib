/*!
 *  @file   utility_datetime.cpp
 *  @brief  [common]日時関連Utility
 *  @date   2017/12/19
 */
#include "utility/utility_datetime.h"

#include "hhmmss.h"
#include "yymmdd.h"
#include "garnet_time.h"

#include <chrono>
#include <codecvt>
#include <ctime>
#include <sstream>
#include "boost/date_time.hpp"
#include "boost/date_time/local_time/local_time.hpp"

namespace
{
const int32_t DAYS_OF_1WEEK = 7;                // 一週間=7日
const int32_t HOURS_OF_1DAY = 24;               // 1日=24時間
const int32_t MINUTES_OF_1HOUR = 60;            // 1時間=60分
const int32_t SECONDS_OF_1MINUTE = 60;          // 1分=60秒
const int64_t MILISECONDS_OF_1SECOND = 1000;    // 1秒=1000ミリ秒

// 1時間の秒数
const int32_t SECONDS_OF_1HOUR = MINUTES_OF_1HOUR*SECONDS_OF_1MINUTE;
// 1日の秒数
const int32_t SECONDS_OF_1DAY = HOURS_OF_1DAY*MINUTES_OF_1HOUR*SECONDS_OF_1MINUTE;
}

namespace garnet
{
namespace utility_datetime
{

/*!
 *  @brief  RFC1123形式の日時文字列からローカル日時を得る
 *  @param[in]  rfc1123 RFC1123形式日時文字列
 *  @param[out] o_lcdt  ローカル日時格納先
 */
boost::posix_time::ptime ToLocalTimeFromRFC1123(const std::wstring& rfc1123)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
    return ToLocalTimeFromRFC1123(cv.to_bytes(rfc1123));
}
boost::posix_time::ptime ToLocalTimeFromRFC1123(const std::string& rfc1123)
{
    // newしたfacetはストリーム内で解放される(らしい)
    boost::local_time::local_time_input_facet* inputFacet
        = new boost::local_time::local_time_input_facet();
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), inputFacet));
    ss.str(rfc1123);
    inputFacet->format("%a, %d %b %Y %H:%M:%S %ZP");

    // ストリームからldtに変換すると設定したzone_ptrが潰されてしまうので
    // 変換したldtからはUTCだけを得て、別途用意したlocalのtime_zoneからオフセットを得る
    // (正しい使い方と思われるやり方は全部駄目だった)
    boost::local_time::local_date_time ldt(
        boost::local_time::local_date_time::utc_time_type(), nullptr);
    ss >> ldt;
    boost::local_time::time_zone_ptr local_zone(new boost::local_time::posix_time_zone("GMT+09"));
    return ldt.utc_time() + local_zone->base_utc_offset() + local_zone->dst_offset();
}

/*!
 *  @brief  RFC1123形式日次文字列に変換
 *  @param  date    年月日
 *  @param  time    時刻
 *  @param  zone    タイムゾーン文字列
 */
std::string ToRFC1123(const garnet::YYMMDD& date,
                      const garnet::HHMMSS& time,
                      const std::string& zone)
{
    std::tm t_tm;
    {
        garnet::sTime t_time;
        t_time.set(date);
        t_time.set(time);
        t_time.copy(t_tm);
        std::mktime(&t_tm); // 曜日補正
    }
    std::ostringstream ss;
    ss << std::put_time(&t_tm, "%a, %d %b %Y %H:%M:%S");
    //
    if (!zone.empty()) {
        return ss.str() + " " + zone;
    } else {
        return ss.str();
    }
}
std::wstring ToRFC1123(const garnet::YYMMDD& date,
                       const garnet::HHMMSS& time,
                       const std::wstring& zone)
{
    std::string str(std::move(ToRFC1123(date, time, std::string())));
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utfconv;
    if (!zone.empty()) {
        return utfconv.from_bytes(str) + L" " + zone;
    } else {
        return utfconv.from_bytes(str);
    }
}

/*!
 *  @brief  指定フォーマットの日時文字列をtmに変換
 *  @param[in]  src     日時文字列
 *  @param[in]  format  ↑形式 (%H:%M:%S など)
 *  @param[out] o_tm    格納先
 *  @note   ローカル変換なし
 */
bool ToTimeFromString(const std::string& src, const std::string& format, garnet::sTime& o_tm)
{
    std::tm t_tm;
    std::istringstream ss(src);
    ss >> std::get_time(&t_tm, format.c_str());
    o_tm = std::move(t_tm);

    return !ss.fail();
}
/*!
 *  @brief  ptimeからtmに変換
 *  @param[in]  src     ptime
 *  @param[out] o_tm    格納先
 *  @param[in]  tm::tm_isdstは常にfalse(サマータイム考慮しない)
 */
void ToTimeFromBoostPosixTime(const boost::posix_time::ptime& src, garnet::sTime& o_tm)
{
    boost::gregorian::date d = src.date();
    boost::posix_time::time_duration t = src.time_of_day();

    o_tm.tm_sec = static_cast<int32_t>(t.seconds());
    o_tm.tm_min = static_cast<int32_t>(t.minutes());
    o_tm.tm_hour = static_cast<int32_t>(t.hours());
    o_tm.tm_mday = d.day();
    o_tm.tm_mon  = d.month() - 1;
    o_tm.tm_year = d.year() - 1900;
    o_tm.tm_wday = d.day_of_week().as_number();
    o_tm.tm_yday = d.day_of_year();
    o_tm.tm_isdst = false;
}


/*!
 *  @brief  経過時間を得る
 *  @return tickCount(ミリ秒)
 */
int64_t GetTickCountGeneral()
{
    auto time_point = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch()).count();
}


/*!
 *  @brief  srcとシステムローカル時間との差を秒[絶対値]で得る
 *  @param  src 比べる時間
 *  @return 秒差
 */
uint32_t GetDiffSecondsFromLocalMachineTime(const garnet::sTime& src)
{
    std::tm src_tm;
    src.copy(src_tm);
    time_t tt_lc = std::time(nullptr);
    time_t tt_src = std::mktime(&src_tm);
    int32_t tt_dif = static_cast<int32_t>(tt_lc - tt_src);
    return std::abs(tt_dif);
}
/*!
 *  @brief  システムローカル時間を文字列で得る
 *  @return 文字列
 */
std::wstring GetLocalMachineTime(const std::wstring& format)
{
    std::wostringstream ss;

    const time_t tt_lc = std::time(nullptr);
    std::tm tm_lc;
#if defined(_WINDOWS)
    localtime_s(&tm_lc, &tt_lc);
#else
    tm_lc = *std::localtime(&tt_lc);
#endif/* defined(_WINDOWS) */
    ss << std::put_time(&tm_lc, format.c_str());

    return ss.str();
}


/*!
 *  @brief  rightとleftの差を秒数で得る
 *  @param  right
 *  @param  left
 */
std::time_t DiffTimeBySecond(const garnet::sTime& right, const garnet::sTime& left)
{
    std::tm right_tm;
    std::tm left_tm;
    right.copy(right_tm);
    left.copy(left_tm);
    const std::time_t right_tt = std::mktime(&right_tm);
    const std::time_t left_tt = std::mktime(&left_tm);
    return std::abs(right_tt - left_tt);
}
/*!
 *  @brief  rightとleftの差を日数で得る
 *  @param  right
 *  @param  left
 */
int32_t DiffTimeByDay(const garnet::sTime& right, const garnet::sTime& left)
{
    const std::time_t sec = DiffTimeBySecond(right, left);
    return static_cast<int32_t>(sec/static_cast<std::time_t>(SECONDS_OF_1DAY));
}


/*!
 *  @brief  年月日時分秒に差分[ミリ秒]を足す
 *  @param[in]  base_tm ベース
 *  @param[in]  diff_ms 差分[ミリ秒]
 *  @param[out] o_now   格納先
 *  @note   mktimeもlocaltimeもローカルタイム処理
 *  @note   localeは未設定でもJSTかGMTかUCTしかありえず、どれが来ても差分を取る分には辻褄合うのでOK
 */
void AddTimeAndDiffMS(const garnet::sTime& base_tm, int64_t diff_ms, garnet::sTime& o_now)
{
    std::tm base_tm_cpy;
    base_tm.copy(base_tm_cpy);
    time_t tt_base = std::mktime(&base_tm_cpy);
    time_t tt_after = tt_base + static_cast<time_t>(diff_ms/MILISECONDS_OF_1SECOND);
#if defined(_WINDOWS)
    std::tm o_tm;
    localtime_s(&o_tm, &tt_after);
    o_now = std::move(o_tm);
#else
    o_now = *std::localtime(&tt_after);
#endif/* defined(_WINDOWS) */
}


/*!
 *  @brief  起点日時からafter_day後の00:00までの時間をミリ秒で得る
 *  @param  base_tm     起点日時
 *  @param  after_day   何日後か(翌日が1)
 *  @note   mktimeは1月32日など存在しない月日を引数に渡しても正しい月日に読み替えて処理してくれる
 */
int64_t GetAfterDayLimitMS(const garnet::sTime& base_tm, int32_t after_day)
{
    if (after_day <= 0) {
        return 0;
    }
    std::tm src_tm;
    std::tm after_tm;
    base_tm.copy(src_tm);
    base_tm.copy(after_tm);
    after_tm.tm_mday += after_day;
    after_tm.tm_hour = 0;
    after_tm.tm_min  = 0;
    after_tm.tm_sec  = 0;
    const std::time_t src_tt = std::mktime(&src_tm);
    const std::time_t after_tt = std::mktime(&after_tm);
    return ToMiliSecondsFromSecond(static_cast<int32_t>(after_tt - src_tt));
}
/*!
 *  @brief  分をミリ秒で得る
 *  @param  minute  任意時間[分]
 */
int64_t ToMiliSecondsFromMinute(int32_t minute)
{
    return static_cast<int64_t>(minute)*ToMiliSecondsFromSecond(SECONDS_OF_1MINUTE);
}
/*!
 *  @brief  秒をミリ秒で得る
*  @param  second  任意時間[秒]
  */
int64_t ToMiliSecondsFromSecond(int32_t second)
{
    return static_cast<int64_t>(second)*MILISECONDS_OF_1SECOND;
}

/*!
 *  @brief  1時間の秒数
 */
int32_t SecondsOf1Hour()
{
    return SECONDS_OF_1HOUR;
}
/*!
 *  @brief  1分の秒数
 */
int32_t SecondsOf1Minute()
{
    return SECONDS_OF_1MINUTE;
}

} // namespace utility_datetime

sTime::sTime(const std::tm& src)
: tm_sec(src.tm_sec)
, tm_min(src.tm_min)
, tm_hour(src.tm_hour)
, tm_mday(src.tm_mday)
, tm_mon(src.tm_mon)
, tm_year(src.tm_year)
, tm_wday(src.tm_wday)
, tm_yday(src.tm_yday)
, tm_isdst(src.tm_isdst)
{
    static_assert(sizeof(std::tm)==sizeof(garnet::sTime), "");
}

sTime::sTime(std::tm&& src)
: tm_sec(src.tm_sec)
, tm_min(src.tm_min)
, tm_hour(src.tm_hour)
, tm_mday(src.tm_mday)
, tm_mon(src.tm_mon)
, tm_year(src.tm_year)
, tm_wday(src.tm_wday)
, tm_yday(src.tm_yday)
, tm_isdst(src.tm_isdst)
{
}

void sTime::copy(std::tm& dst) const
{
    dst.tm_sec = tm_sec;
    dst.tm_min = tm_min;
    dst.tm_hour = tm_hour;
    dst.tm_mday = tm_mday;
    dst.tm_mon = tm_mon;
    dst.tm_year = tm_year;
    dst.tm_wday = tm_wday;
    dst.tm_yday = tm_yday;
    dst.tm_isdst = tm_isdst;
}

sTime sTime::operator+(const sTime& right)
{
    std::tm sum;
    sum.tm_sec = tm_sec + right.tm_sec;
    sum.tm_min = tm_min + right.tm_min;
    sum.tm_hour = tm_hour + right.tm_hour;
    sum.tm_mday = tm_mday + right.tm_mday;
    sum.tm_mon = tm_mon + right.tm_mon;
    sum.tm_year = tm_year + right.tm_year;
    std::mktime(&sum);
    return sum;
}

} // namespace garnet
