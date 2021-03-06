/*!
 *  @file   hhmmss.h
 *  @brief  [common]時分秒だけの最少時間構成
 *  @date   2017/12/27
 */
#pragma once

#include <string>

namespace garnet
{
struct sTime;

struct HHMMSS
{
    int32_t m_hour;
    int32_t m_minute;
    int32_t m_second;

    HHMMSS()
    : m_hour(0)
    , m_minute(0)
    , m_second(0)
    {
    }

    HHMMSS(const garnet::sTime&);
    HHMMSS(garnet::sTime&&);

    /*!
     *  @brief  "HH:MM:SS"形式の年月日文字列から生成
     *  @param  src 時分秒文字列
     */
    static HHMMSS Create(const std::string& src);

    /*!
     *  @brief  文字列で得る
     */
    std::string to_string() const;
    /*!
     *  @brief  ':'区切りの文字列で得る
     */
    std::string to_delim_string() const;

    /*!
     *  @brief  00:00:00からの経過秒数を得る
     */
    int32_t GetPastSecond() const;
    /*!
     *  @brief  引き算
     *  @return this-rightを秒単位で返す
     */
    int32_t Sub(const HHMMSS& right) const
    {
        return GetPastSecond() - right.GetPastSecond();
    }
    bool operator<(const HHMMSS& right) const
    {
        return GetPastSecond() < right.GetPastSecond();
    }
    bool operator>(const HHMMSS& right) const
    {
        return GetPastSecond() > right.GetPastSecond();
    }
    bool operator<=(const HHMMSS& right) const
    {
        return GetPastSecond() <= right.GetPastSecond();
    }
};

} // namespace garnet
