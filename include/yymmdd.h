/*!
 *  @file   yymmdd.h
 *  @brief  [common]年月日だけの最少時間構成
 *  @date   2017/12/30
 */
#pragma once

#include <string>

namespace garnet
{
struct sTime;

/*!
 *  @brief  年月
 */
struct YYMM
{
    int32_t m_year;     //!< 年(西暦)
    int32_t m_month;    //!< 暦月

    YYMM()
    : m_year(0)
    , m_month(0)
    {
    }

    YYMM(int32_t year, int32_t month)
    : m_year(year)
    , m_month(month)
    {
    }

    YYMM(const garnet::sTime&);
    YYMM(const garnet::YYMM&);

    /*!
     *  @brief  "YYYY/MM"形式の年月日文字列から生成
     *  @param  src 年月文字列(1900年以降)
     *  @note   西暦部分がが3桁以下ならば2000年代の省略形とみなす
     *  @note   ("10/01"は2010年1月)
     */
    static YYMM Create(const std::string& src);

    /*!
     *  @brief  文字列で得る
     *  @return YYYYMM
     */
    std::string to_string() const;
    /*!
     *  @brief  '/'区切りの文字列で得る
     *  @return YYYY/MM
     */
    std::string to_delim_string() const;

    /*!
     *  @brief  月インクリメント(書き換え)
     */
    void inc_month();
    /*!
     *  @brief  前月を得る
     */
    YYMM prev() const;

    bool operator>(const YYMM& right) const
    {
        return (m_year > right.m_year || (m_year == right.m_year && m_month > right.m_month));
    }
    bool operator>=(const YYMM& right) const
    {
        return (m_year > right.m_year || (m_year == right.m_year && m_month >= right.m_month));
    }
    bool operator<=(const YYMM& right) const
    {
        return (right.m_year > m_year || (m_year == right.m_year && right.m_month >= m_month));
    }
};

/*!
 *  @brief  月日
 */
struct MMDD
{
    int32_t m_month;    //! 歴月
    int32_t m_day;      //! 歴日

    MMDD()
    : m_month(0)
    , m_day(0)
    {
    }

    MMDD(const garnet::sTime&);
    MMDD(garnet::sTime&&);

    /*!
     *  @brief  "MM/DD"形式の月日文字列から生成
     *  @param  src 月日文字列
     */
    static MMDD Create(const std::string& src);

    /*!
     *  @brief  文字列で得る
     *  @return MMDD
     */
    std::string to_string() const;
    /*!
     *  @brief  '/'区切りの文字列で得る
     *  @return MM/DD
     */
    std::string to_delim_string() const;
    /*!
     *  @brief  '-'区切りの文字列で得る
     *  @return MM-DD
     */
    std::string to_hyphen_string() const;

    bool operator==(const MMDD& right) const
    {
        return (m_month == right.m_month && m_day == right.m_day);
    }

    bool empty() const
    {
        return m_month == 0; // "0月"は引数なし生成時の初期化値なので空とみなす
    }
};

/*!
 *  @brief  年月日
 */
struct YYMMDD : public MMDD
{
    int32_t m_year; //!< 年(西暦)

    YYMMDD()
    : MMDD()
    , m_year(0)
    {
    }

    YYMMDD(const garnet::sTime&);
    YYMMDD(garnet::sTime&&);

    /*!
     *  @brief  "YYYY/MM/DD"形式の年月日文字列から生成
     *  @param  src 年月日文字列(1900年以降)
     *  @note   西暦部分がが3桁以下ならば2000年代の省略形とみなす
     *  @note   ("10/01/01"は2010年1月1日)
     */
    static YYMMDD Create(const std::string& src);

    /*!
     *  @brief  文字列で得る
     *  @return YYYYMMDD
     */
    std::string to_string() const;
    /*!
     *  @brief  '/'区切りの文字列で得る
     *  @return YYYY/MM/DD
     */
    std::string to_delim_string() const;
    /*!
     *  @brief  '-'区切りの文字列で得る
     *  @return YYYY-MM-DD
     */
    std::string to_hyphen_string() const;

    /*!
     *  @brief  num日後のYYMMDDを得る
     *  @param  num 日数
     */
    YYMMDD GetAfterDay(int32_t num) const;
    /*!
     *  @brief  dstとの日数差(絶対値)を得る
     *  @param  dst 比較年月日
     */
    int32_t GetDiffDay(const YYMMDD& dst) const;

    bool operator==(const YYMMDD& right) const
    {
        if (MMDD::operator==(right)) {
            return m_year == right.m_year;
        } else {
            return false;
        }
    }
};

} // namespace garnet
