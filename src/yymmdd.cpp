/*!
 *  @file   yymmdd.cpp
 *  @brief  [common]�N���������̍ŏ����ԍ\��
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

const int32_t CALENDER_MONTH_FIRST = 1;     //! �擪�
const int32_t CALENDER_MONTH_LAST = 12;     //! �ŏI�

//! tm��0�N�ɑ������鐼��N
const int32_t TM_BASE_AD = 1900;
//! tm��0���ɑ�������
const int32_t TM_BASE_CALENDER_MONTH = CALENDER_MONTH_FIRST;
//! ����2000�N�ɑ�������tm�N
const int32_t TM_YEAR_2000 = 100;


/*!
 *  @brief  �����𕶎���ɕϊ�
 *  @param  number  �������l
 *  @note   1���Ȃ�0���߁A2���Ȃ炻�̂܂ܕԂ�
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
 *  @brief  �N(tm_year)�𐼗�œ���
 */
int32_t sTime::to_AD() const
{
    return tm_year + TM_BASE_AD;
}
/*!
 *  @brief  ��(tm_month)���œ���
 */
int32_t sTime::to_calender_month() const
{
    return tm_mon + TM_BASE_CALENDER_MONTH;
}
/*!
 *  @brief  MMDD�̃p�����[�^���Z�b�g����
 *  @param  src
 */
void sTime::set(const MMDD& src)
{
    tm_mon = src.m_month - TM_BASE_CALENDER_MONTH;
    tm_mday = src.m_day;
}
/*!
 *  @brief  YYMMDD�̃p�����[�^���Z�b�g����
 *  @param  src
 */
void sTime::set(const YYMMDD& src)
{
    set(static_cast<const MMDD&>(src));
    tm_year = src.m_year - TM_BASE_AD;
}



/*!
 *  @param  stime   �N���������b�p�����[�^
 */
YYMM::YYMM(const garnet::sTime& stime)
: m_year(stime.to_AD())
, m_month(stime.to_calender_month())
{
}
/*!
 *  @param  yymm    �N��
 */
YYMM::YYMM(const garnet::YYMM& yymm)
: m_year(yymm.m_year)
, m_month(yymm.m_month)
{
}

/*!
 *  @param  src "YYYY/MM"�`���̔N��������
 */
YYMM YYMM::Create(const std::string& src)
{
    garnet::sTime yymm_tm;
    if (utility_datetime::ToTimeFromString(src, "%Y/%m", yymm_tm)) {
        // ��N4��������2000�N�ȍ~�̏ȗ��`�Ƃ݂Ȃ�
        if (src.find(DELIM_C) < 4) {
            yymm_tm.tm_year += TM_YEAR_2000;
        }
        return YYMM(yymm_tm);
    } else {
        return YYMM();
    }
}

/*!
 *  @brief  ������œ���
 */
std::string YYMM::to_string() const
{
    return std::to_string(m_year) + mon_day_to_str(m_month);
}
/*!
 *  @brief  '/'��؂�̕�����𓾂�
 */
std::string YYMM::to_delim_string() const
{
    return std::to_string(m_year) + DELIM + mon_day_to_str(m_month);
}

/*!
 *  @brief  ���C���N�������g
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
 *  @param  tm  �N���������b�p�����[�^
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
 *  @param  src "MM/DD"�`���̌���������
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
 *  @brief  ������œ���
 */
std::string MMDD::to_string() const
{
    return mon_day_to_str(m_month) + mon_day_to_str(m_day);
}
/*!
 *  @brief  '/'��؂�̕�����𓾂�
 */
std::string MMDD::to_delim_string() const
{
    return mon_day_to_str(m_month) + DELIM + mon_day_to_str(m_day);
}
/*!
 *  @brief  '-'��؂�̕�����𓾂�
 */
std::string MMDD::to_hyphen_string() const
{
    return mon_day_to_str(m_month) + HYPHEN + mon_day_to_str(m_day);
}


/*!
 *  @param  tm  �N���������b�p�����[�^
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
 *  @param  src "YYYY/MM/DD"�`���̔N����������
 */
YYMMDD YYMMDD::Create(const std::string& src)
{
    garnet::sTime yymmdd_tm;
    if (utility_datetime::ToTimeFromString(src, "%Y/%m/%d", yymmdd_tm)) {
        // ��N4��������2000�N�ȍ~�̏ȗ��`�Ƃ݂Ȃ�
        if (src.find(DELIM_C) < 4) {
            yymmdd_tm.tm_year += TM_YEAR_2000;
        }
        return YYMMDD(yymmdd_tm);
    } else {
        return YYMMDD();
    }
}

/*!
 *  @brief  ������œ���
 */
std::string YYMMDD::to_string() const
{
    return std::to_string(m_year) + MMDD::to_string();
}
/*!
 *  @brief  '/'��؂�̕�����œ���
 */
std::string YYMMDD::to_delim_string() const
{
    return std::to_string(m_year) + DELIM + MMDD::to_delim_string();
}
/*!
 *  @brief  '-'��؂�̕�����œ���
 */
std::string YYMMDD::to_hyphen_string() const
{
    return std::to_string(m_year) + HYPHEN + MMDD::to_hyphen_string();
}

/*!
 *  @brief  num�����YYMMDD�𓾂�
 *  @param  num ����
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
 *  @brief  dst�Ƃ̓�����(��Βl)�𓾂�
 *  @param  dst ��r�N����
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
