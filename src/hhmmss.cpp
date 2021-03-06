/*!
 *  @file   yymmdd.cpp
 *  @brief  [common]年月日だけの最少時間構成
 *  @date   2018/01/16
 */
#include "hhmmss.h"

#include "utility/utility_datetime.h"
#include "garnet_time.h"

namespace garnet
{

namespace
{
const char DELIM_C = ':';
const char DELIM[] = ":";
}


/*!
 *  @param  stime   年月日時分秒パラメータ
 */
HHMMSS::HHMMSS(const garnet::sTime& stime)
: m_hour(stime.tm_hour)
, m_minute(stime.tm_min)
, m_second(stime.tm_sec)
{
}
HHMMSS::HHMMSS(garnet::sTime&& stime)
: m_hour(stime.tm_hour)
, m_minute(stime.tm_min)
, m_second(stime.tm_sec)
{
}

/*!
 *  @brief  "HH:MM:SS"形式の年月日文字列から生成
 */
HHMMSS HHMMSS::Create(const std::string& src)
{
    garnet::sTime hhmmss_tm;
    if (utility_datetime::ToTimeFromString(src, "%T", hhmmss_tm)) {
        return HHMMSS(hhmmss_tm);
    } else {
        return HHMMSS();
    }
}

/*!
 *  @brief  00:00:00からの経過秒数を得る
 */
int32_t HHMMSS::GetPastSecond() const
{
    return utility_datetime::SecondsOf1Hour()*m_hour +
           utility_datetime::SecondsOf1Minute()*m_minute +
           m_second;
}

/*!
 *  @brief  文字列で得る
 */
std::string HHMMSS::to_string() const
{
    const std::string zero("0");
    std::string ret;

    if (m_hour < 10) {
        ret += zero;
    }
    ret += std::to_string(m_hour);
    if (m_minute < 10) {
        ret += zero;
    }
    ret += std::to_string(m_minute);
    if (m_second < 10) {
        ret += zero;
    }
    ret += std::to_string(m_second);

    return ret;
}
/*!
 *  @brief  ':'区切りの文字列で得る
 */
std::string HHMMSS::to_delim_string() const
{
    const std::string zero("0");
    const std::string delim(DELIM);
    std::string ret;

    if (m_hour < 10) {
        ret += zero;
    }
    ret += std::to_string(m_hour) + delim;
    if (m_minute < 10) {
        ret += zero;
    }
    ret += std::to_string(m_minute) + delim;
    if (m_second < 10) {
        ret += zero;
    }
    ret += std::to_string(m_second);

    return ret;
}

/*!
 *  @brief  HHMMSSのパラメータをセットする
 *  @param  src
 */
void sTime::set(const HHMMSS& src)
{
    tm_hour = src.m_hour;
    tm_min = src.m_minute;
    tm_sec = src.m_second;
}

} // namespace garnet
