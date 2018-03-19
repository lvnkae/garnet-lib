/*!
 *  @file   yymmdd.h
 *  @brief  [common]�N���������̍ŏ����ԍ\��
 *  @date   2017/12/30
 */
#pragma once

#include <string>

namespace garnet
{
struct sTime;

/*!
 *  @brief  �N��
 */
struct YYMM
{
    int32_t m_year;     //!< �N(����)
    int32_t m_month;    //!< �

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
     *  @brief  "YYYY/MM"�`���̔N���������񂩂琶��
     *  @param  src �N��������(1900�N�ȍ~)
     *  @note   ���������3���ȉ��Ȃ��2000�N��̏ȗ��`�Ƃ݂Ȃ�
     *  @note   ("10/01"��2010�N1��)
     */
    static YYMM Create(const std::string& src);

    /*!
     *  @brief  ������œ���
     *  @return YYYYMM
     */
    std::string to_string() const;
    /*!
     *  @brief  '/'��؂�̕�����œ���
     *  @return YYYY/MM
     */
    std::string to_delim_string() const;

    /*!
     *  @brief  ���C���N�������g(��������)
     */
    void inc_month();
    /*!
     *  @brief  �O���𓾂�
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
 *  @brief  ����
 */
struct MMDD
{
    int32_t m_month;    //! ����
    int32_t m_day;      //! ���

    MMDD()
    : m_month(0)
    , m_day(0)
    {
    }

    MMDD(const garnet::sTime&);
    MMDD(garnet::sTime&&);

    /*!
     *  @brief  "MM/DD"�`���̌��������񂩂琶��
     *  @param  src ����������
     */
    static MMDD Create(const std::string& src);

    /*!
     *  @brief  ������œ���
     *  @return MMDD
     */
    std::string to_string() const;
    /*!
     *  @brief  '/'��؂�̕�����œ���
     *  @return MM/DD
     */
    std::string to_delim_string() const;
    /*!
     *  @brief  '-'��؂�̕�����œ���
     *  @return MM-DD
     */
    std::string to_hyphen_string() const;

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

    YYMMDD(const garnet::sTime&);
    YYMMDD(garnet::sTime&&);

    /*!
     *  @brief  "YYYY/MM/DD"�`���̔N���������񂩂琶��
     *  @param  src �N����������(1900�N�ȍ~)
     *  @note   ���������3���ȉ��Ȃ��2000�N��̏ȗ��`�Ƃ݂Ȃ�
     *  @note   ("10/01/01"��2010�N1��1��)
     */
    static YYMMDD Create(const std::string& src);

    /*!
     *  @brief  ������œ���
     *  @return YYYYMMDD
     */
    std::string to_string() const;
    /*!
     *  @brief  '/'��؂�̕�����œ���
     *  @return YYYY/MM/DD
     */
    std::string to_delim_string() const;
    /*!
     *  @brief  '-'��؂�̕�����œ���
     *  @return YYYY-MM-DD
     */
    std::string to_hyphen_string() const;

    /*!
     *  @brief  num�����YYMMDD�𓾂�
     *  @param  num ����
     */
    YYMMDD GetAfterDay(int32_t num) const;
    /*!
     *  @brief  dst�Ƃ̓�����(��Βl)�𓾂�
     *  @param  dst ��r�N����
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
