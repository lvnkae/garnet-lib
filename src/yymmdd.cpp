/*!
 *  @file   yymmdd.cpp
 *  @brief  [common]年月日だけの最少時間構成
 *  @date   2018/01/16
 */
#include "yymmdd.h"

#include "utility/utility_datetime.h"
#include "garnet_time.h"

namespace garnet
{

namespace
{
const char DELIM_C = '/';
const char DELIM[] = "/";
}

/*!
 *  @param  tm  年月日時分秒パラメータ
 */
MMDD::MMDD(const garnet::sTime& stime)
: m_month(stime.tm_mon+1) // 1始まり
, m_day(stime.tm_mday)
{
}
MMDD::MMDD(garnet::sTime&& stime)
: m_month(stime.tm_mon+1) // 1始まり
, m_day(stime.tm_mday)
{
}

/*!
 *  @param  src "MM/DD"形式の月日文字列
 */
MMDD MMDD::Create(const std::string& src)
{
    garnet::sTime mmdd_tm;
    if (utility_datetime::ToTimeFromString(src, "%m/%d", mmdd_tm)) {
        return MMDD(mmdd_tm);
    } else {
        return MMDD();
    }
}

/*!
 *  @brief  "MM/DD"形式の文字列を得る
 */
std::string MMDD::to_string() const
{
    const std::string zero("0");

    std::string ret;
    if (m_month < 10) {
        ret += zero;
    }
    ret += std::to_string(m_month);
    if (m_day < 10) {
        ret += zero;
    }
    ret += std::to_string(m_day);

    return ret;
}
/*!
 *  @brief  '/'区切りの文字列を得る
 */
std::string MMDD::to_delim_string() const
{
    const std::string zero("0");
    const std::string delim(DELIM);

    std::string ret;
    if (m_month < 10) {
        ret += zero;
    }
    ret += std::to_string(m_month) + delim;
    if (m_day < 10) {
        ret += zero;
    }
    ret += std::to_string(m_day);

    return ret;
}


/*!
 *  @param  tm  年月日時分秒パラメータ
 */
YYMMDD::YYMMDD(const garnet::sTime& stime)
: MMDD(stime)
, m_year(stime.tm_year + 1900) // 西暦
{
}
YYMMDD::YYMMDD(garnet::sTime&& stime)
: MMDD(stime)
, m_year(stime.tm_year + 1900) // 西暦
{
}

/*!
 *  @param  src "YYYY/MM/DD"形式の年月日文字列
 */
YYMMDD YYMMDD::Create(const std::string& src)
{
    garnet::sTime yymmdd_tm;
    if (utility_datetime::ToTimeFromString(src, "%Y/%m/%d", yymmdd_tm)) {
        if (src.find(DELIM_C) < 4) {
            yymmdd_tm.tm_year += 100;
        }
        return YYMMDD(yymmdd_tm);
    } else {
        return YYMMDD();
    }
}

/*!
 *  @brief  文字列で得る
 */
std::string YYMMDD::to_string() const
{
    return std::to_string(m_year) + MMDD::to_string();
}
/*!
 *  @brief  '/'区切りの文字列で得る
 */
std::string YYMMDD::to_delim_string() const
{
    return std::to_string(m_year) + DELIM + MMDD::to_delim_string();
}

} // namespace garnet
