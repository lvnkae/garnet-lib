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
const char HYPHEN_C = '-';
const char HYPHEN[] = "-";

const int32_t CALENDER_MONTH_FIRST = 1;     //! 先頭暦月
const int32_t CALENDER_MONTH_LAST = 12;     //! 最終暦月

//! tmの0年に相当する西暦年
const int32_t TM_BASE_AD = 1900;
//! tmの0月に相当する暦月
const int32_t TM_BASE_CALENDER_MONTH = CALENDER_MONTH_FIRST;
//! 西暦2000年に相当するtm年
const int32_t TM_YEAR_2000 = 100;


/*!
 *  @brief  月日を文字列に変換
 *  @param  number  月日数値
 *  @note   1桁なら0埋め、2桁ならそのまま返す
 */
std::string mon_day_to_str(int32_t number)
{
    if (number < 10) {
        return "0" + std::to_string(number);
    } else {
        return std::to_string(number);
    }   
}

}


/*!
 *  @brief  年(tm_year)を西暦で得る
 */
int32_t sTime::to_AD() const
{
    return tm_year + TM_BASE_AD;
}
/*!
 *  @brief  月(tm_month)を暦月で得る
 */
int32_t sTime::to_calender_month() const
{
    return tm_mon + TM_BASE_CALENDER_MONTH;
}
/*!
 *  @brief  MMDDのパラメータをセットする
 *  @param  src
 */
void sTime::set(const MMDD& src)
{
    tm_mon = src.m_month - TM_BASE_CALENDER_MONTH;
    tm_mday = src.m_day;
}
/*!
 *  @brief  YYMMDDのパラメータをセットする
 *  @param  src
 */
void sTime::set(const YYMMDD& src)
{
    set(static_cast<const MMDD&>(src));
    tm_year = src.m_year - TM_BASE_AD;
}



/*!
 *  @param  stime   年月日時分秒パラメータ
 */
YYMM::YYMM(const garnet::sTime& stime)
: m_year(stime.to_AD())
, m_month(stime.to_calender_month())
{
}
/*!
 *  @param  yymm    年月
 */
YYMM::YYMM(const garnet::YYMM& yymm)
: m_year(yymm.m_year)
, m_month(yymm.m_month)
{
}

/*!
 *  @param  src "YYYY/MM"形式の年月文字列
 */
YYMM YYMM::Create(const std::string& src)
{
    garnet::sTime yymm_tm;
    if (utility_datetime::ToTimeFromString(src, "%Y/%m", yymm_tm)) {
        // 暦年4桁未満は2000年以降の省略形とみなす
        if (src.find(DELIM_C) < 4) {
            yymm_tm.tm_year += TM_YEAR_2000;
        }
        return YYMM(yymm_tm);
    } else {
        return YYMM();
    }
}

/*!
 *  @brief  文字列で得る
 */
std::string YYMM::to_string() const
{
    return std::to_string(m_year) + mon_day_to_str(m_month);
}
/*!
 *  @brief  '/'区切りの文字列を得る
 */
std::string YYMM::to_delim_string() const
{
    return std::to_string(m_year) + DELIM + mon_day_to_str(m_month);
}

/*!
 *  @brief  月インクリメント
 */
void YYMM::inc_month()
{
    if (m_month == CALENDER_MONTH_LAST) {
        m_year++;
        m_month = CALENDER_MONTH_FIRST;
    } else {
        m_month++;
    }
}


/*!
 *  @param  tm  年月日時分秒パラメータ
 */
MMDD::MMDD(const garnet::sTime& stime)
: m_month(stime.to_calender_month())
, m_day(stime.tm_mday)
{
}
MMDD::MMDD(garnet::sTime&& stime)
: m_month(stime.to_calender_month())
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
 *  @brief  文字列で得る
 */
std::string MMDD::to_string() const
{
    return mon_day_to_str(m_month) + mon_day_to_str(m_day);
}
/*!
 *  @brief  '/'区切りの文字列を得る
 */
std::string MMDD::to_delim_string() const
{
    return mon_day_to_str(m_month) + DELIM + mon_day_to_str(m_day);
}
/*!
 *  @brief  '-'区切りの文字列を得る
 */
std::string MMDD::to_hyphen_string() const
{
    return mon_day_to_str(m_month) + HYPHEN + mon_day_to_str(m_day);
}


/*!
 *  @param  tm  年月日時分秒パラメータ
 */
YYMMDD::YYMMDD(const garnet::sTime& stime)
: MMDD(stime)
, m_year(stime.to_AD())
{
}
YYMMDD::YYMMDD(garnet::sTime&& stime)
: MMDD(stime)
, m_year(stime.to_AD())
{
}

/*!
 *  @param  src "YYYY/MM/DD"形式の年月日文字列
 */
YYMMDD YYMMDD::Create(const std::string& src)
{
    garnet::sTime yymmdd_tm;
    if (utility_datetime::ToTimeFromString(src, "%Y/%m/%d", yymmdd_tm)) {
        // 暦年4桁未満は2000年以降の省略形とみなす
        if (src.find(DELIM_C) < 4) {
            yymmdd_tm.tm_year += TM_YEAR_2000;
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
/*!
 *  @brief  '-'区切りの文字列で得る
 */
std::string YYMMDD::to_hyphen_string() const
{
    return std::to_string(m_year) + HYPHEN + MMDD::to_hyphen_string();
}

/*!
 *  @brief  num日後のYYMMDDを得る
 *  @param  num 日数
 */
YYMMDD YYMMDD::GetAfterDay(int32_t num) const
{
    sTime src;
    src.set(*this);
    sTime dst;
    dst.tm_mday = num;
    return src + dst;
}
/*!
 *  @brief  dstとの日数差(絶対値)を得る
 *  @param  dst 比較年月日
 */
int32_t YYMMDD::GetDiffDay(const YYMMDD& dst) const
{
    sTime right;
    right.set(*this);
    sTime left;
    left.set(dst);
    return utility_datetime::DiffTimeByDay(right, left);
}

} // namespace garnet
