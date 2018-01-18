/*!
 *  @file   garnet_time.h
 *  @brief  [common]���Ԋ֘A
 *  @date   2018/01/12
 */
#pragma once

namespace std { struct tm; }

namespace garnet
{
struct MMDD;
struct YYMMDD;
struct HHMMSS;

/*!
 *  @brief  tm�̑�p�\����
 *  @note   VS��std::tm�͊����������w�b�_�ő��d�錾�G���[���o�₷��
 *  @note   std::tm��v�����Ă�std�֐��ȊO�͂��������g���c
 */
struct sTime
{
    int32_t tm_sec;     /* seconds after the minute - [0,59] */
    int32_t tm_min;     /* minutes after the hour - [0,59] */
    int32_t tm_hour;    /* hours since midnight - [0,23] */
    int32_t tm_mday;    /* day of the month - [1,31] */
    int32_t tm_mon;     /* months since January - [0,11] */
    int32_t tm_year;    /* years since 1900 */
    int32_t tm_wday;    /* days since Sunday - [0,6] */
    int32_t tm_yday;    /* days since January 1 - [0,365] */
    int32_t tm_isdst;   /* daylight savings time flag */

    sTime()
    : tm_sec(0)
    , tm_min(0)
    , tm_hour(0)
    , tm_mday(0)
    , tm_mon(0)
    , tm_year(0)
    , tm_wday(0)
    , tm_yday(0)
    , tm_isdst(0)
    {
    }

    sTime(const std::tm&);
    sTime(std::tm&&);

    void set(const MMDD& src);
    void set(const YYMMDD& src);
    void set(const HHMMSS& src);

    void copy(std::tm&) const;
};

} // namespace garnet
