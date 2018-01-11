/*!
 *  @file   yymmdd.h
 *  @brief  [common]�N���������̍ŏ����ԍ\��
 *  @date   2017/12/30
 */
#pragma once

#include <string>
#include <ctime>

namespace garnet
{

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

    MMDD(const std::tm&);

    /*!
     *  @brief  "MM/DD"�`���̌��������񂩂琶��
     *  @param  src ����������
     */
    static MMDD Create(const std::string& src);

    bool operator==(const MMDD& right) const
    {
        return (m_month == right.m_month && m_day == right.m_day);
    }

    bool empty() const
    {
        return m_month == 0; // "0��"�͈����Ȃ��������̏������l�Ȃ̂ŋ�Ƃ݂Ȃ�
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

    YYMMDD(const std::tm&);

    /*!
     *  @brief  "YYYY/MM/DD"�`���̔N���������񂩂琶��
     *  @param  src �N����������(1900�N�ȍ~)
     *  @note   ���������3���ȉ��Ȃ��2000�N��̏ȗ��`�Ƃ݂Ȃ�
     *  @note   ("10/01/01"��2010�N1��1��)
     */
    static YYMMDD Create(const std::string& src);

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
