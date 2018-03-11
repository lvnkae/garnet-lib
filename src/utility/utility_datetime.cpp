/*!
 *  @file   utility_datetime.cpp
 *  @brief  [common]�����֘AUtility
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
const int32_t DAYS_OF_1WEEK = 7;                // ��T��=7��
const int32_t HOURS_OF_1DAY = 24;               // 1��=24����
const int32_t MINUTES_OF_1HOUR = 60;            // 1����=60��
const int32_t SECONDS_OF_1MINUTE = 60;          // 1��=60�b
const int64_t MILISECONDS_OF_1SECOND = 1000;    // 1�b=1000�~���b

// 1���Ԃ̕b��
const int32_t SECONDS_OF_1HOUR = MINUTES_OF_1HOUR*SECONDS_OF_1MINUTE;
// 1���̕b��
const int32_t SECONDS_OF_1DAY = HOURS_OF_1DAY*MINUTES_OF_1HOUR*SECONDS_OF_1MINUTE;
}

namespace garnet
{
namespace utility_datetime
{

/*!
 *  @brief  RFC1123�`���̓��������񂩂烍�[�J�������𓾂�
 *  @param[in]  rfc1123 RFC1123�`������������
 *  @param[out] o_lcdt  ���[�J�������i�[��
 */
boost::posix_time::ptime ToLocalTimeFromRFC1123(const std::wstring& rfc1123)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
    return ToLocalTimeFromRFC1123(cv.to_bytes(rfc1123));
}
boost::posix_time::ptime ToLocalTimeFromRFC1123(const std::string& rfc1123)
{
    // new����facet�̓X�g���[�����ŉ�������(�炵��)
    boost::local_time::local_time_input_facet* inputFacet
        = new boost::local_time::local_time_input_facet();
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), inputFacet));
    ss.str(rfc1123);
    inputFacet->format("%a, %d %b %Y %H:%M:%S %ZP");

    // �X�g���[������ldt�ɕϊ�����Ɛݒ肵��zone_ptr���ׂ���Ă��܂��̂�
    // �ϊ�����ldt�����UTC�����𓾂āA�ʓr�p�ӂ���local��time_zone����I�t�Z�b�g�𓾂�
    // (�������g�����Ǝv��������͑S���ʖڂ�����)
    boost::local_time::local_date_time ldt(
        boost::local_time::local_date_time::utc_time_type(), nullptr);
    ss >> ldt;
    boost::local_time::time_zone_ptr local_zone(new boost::local_time::posix_time_zone("GMT+09"));
    return ldt.utc_time() + local_zone->base_utc_offset() + local_zone->dst_offset();
}

/*!
 *  @brief  RFC1123�`������������ɕϊ�
 *  @param  date    �N����
 *  @param  time    ����
 *  @param  zone    �^�C���]�[��������
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
        std::mktime(&t_tm); // �j���␳
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
 *  @brief  �w��t�H�[�}�b�g�̓����������tm�ɕϊ�
 *  @param[in]  src     ����������
 *  @param[in]  format  ���`�� (%H:%M:%S �Ȃ�)
 *  @param[out] o_tm    �i�[��
 *  @note   ���[�J���ϊ��Ȃ�
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
 *  @brief  ptime����tm�ɕϊ�
 *  @param[in]  src     ptime
 *  @param[out] o_tm    �i�[��
 *  @param[in]  tm::tm_isdst�͏��false(�T�}�[�^�C���l�����Ȃ�)
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
 *  @brief  �o�ߎ��Ԃ𓾂�
 *  @return tickCount(�~���b)
 */
int64_t GetTickCountGeneral()
{
    auto time_point = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch()).count();
}


/*!
 *  @brief  src�ƃV�X�e�����[�J�����ԂƂ̍���b[��Βl]�œ���
 *  @param  src ��ׂ鎞��
 *  @return �b��
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
 *  @brief  �V�X�e�����[�J�����Ԃ𕶎���œ���
 *  @return ������
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
 *  @brief  right��left�̍���b���œ���
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
 *  @brief  right��left�̍�������œ���
 *  @param  right
 *  @param  left
 */
int32_t DiffTimeByDay(const garnet::sTime& right, const garnet::sTime& left)
{
    const std::time_t sec = DiffTimeBySecond(right, left);
    return static_cast<int32_t>(sec/static_cast<std::time_t>(SECONDS_OF_1DAY));
}


/*!
 *  @brief  �N���������b�ɍ���[�~���b]�𑫂�
 *  @param[in]  base_tm �x�[�X
 *  @param[in]  diff_ms ����[�~���b]
 *  @param[out] o_now   �i�[��
 *  @note   mktime��localtime�����[�J���^�C������
 *  @note   locale�͖��ݒ�ł�JST��GMT��UCT�������肦���A�ǂꂪ���Ă���������镪�ɂ͒��덇���̂�OK
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
 *  @brief  �N�_��������after_day���00:00�܂ł̎��Ԃ��~���b�œ���
 *  @param  base_tm     �N�_����
 *  @param  after_day   �����ォ(������1)
 *  @note   mktime��1��32���ȂǑ��݂��Ȃ������������ɓn���Ă������������ɓǂݑւ��ď������Ă����
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
 *  @brief  �����~���b�œ���
 *  @param  minute  �C�ӎ���[��]
 */
int64_t ToMiliSecondsFromMinute(int32_t minute)
{
    return static_cast<int64_t>(minute)*ToMiliSecondsFromSecond(SECONDS_OF_1MINUTE);
}
/*!
 *  @brief  �b���~���b�œ���
*  @param  second  �C�ӎ���[�b]
  */
int64_t ToMiliSecondsFromSecond(int32_t second)
{
    return static_cast<int64_t>(second)*MILISECONDS_OF_1SECOND;
}

/*!
 *  @brief  1���Ԃ̕b��
 */
int32_t SecondsOf1Hour()
{
    return SECONDS_OF_1HOUR;
}
/*!
 *  @brief  1���̕b��
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
