/*!
 *  @file   yymmdd.h
 *  @brief  [common]�N���������̍ŏ����ԍ\��
 *  @date   2017/12/30
 */
#pragma once

/*!
 *  @brief  ����
 */
struct MMDD
{
    int32_t m_month;    //! ��(1�n�܂�)
    int32_t m_day;      //! ��(1�n�܂�)

    MMDD()
    : m_month(0)
    , m_day(0)
    {
    }

    MMDD(int32_t m, int32_t d)
    : m_month(m)
    , m_day(d)
    {
    }

    bool operator==(const MMDD& right) const
    {
        return (m_month == right.m_month && m_day == right.m_day);
    }
};

/*!
 *  @brief  �N����
 */
struct YYMMDD : public MMDD
{
    int32_t m_year; //!< �N(����)

    YYMMDD()
    : MMDD()
    , m_year(0)
    {
    }

    YYMMDD(int32_t y, int32_t m, int32_t d)
    : MMDD(m, d)
    , m_year(y)
    {
    }

    bool operator==(const YYMMDD& right) const
    {
        if (MMDD::operator==(right)) {
            return m_year == right.m_year;
        } else {
            return false;
        }
    }
};
