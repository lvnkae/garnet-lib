/*!
 *  @file   yymmdd.cpp
 *  @brief  [common]�N���������̍ŏ����ԍ\��
 *  @date   2018/01/16
 */
#include "hhmmss.h"

#include "utility/utility_datetime.h"
#include "garnet_time.h"

namespace garnet
{

namespace
{
const char DELIM_C = ':';
const char DELIM[] = ":";
}


/*!
 *  @param  stime   �N���������b�p�����[�^
 */
HHMMSS::HHMMSS(const garnet::sTime& stime)
: m_hour(stime.tm_hour)
, m_minute(stime.tm_min)
, m_second(stime.tm_sec)
{
}
HHMMSS::HHMMSS(garnet::sTime&& stime)
: m_hour(stime.tm_hour)
, m_minute(stime.tm_min)
, m_second(stime.tm_sec)
{
}

/*!
 *  @brief  00:00:00����̌o�ߕb���𓾂�
 */
int32_t HHMMSS::GetPastSecond() const
{
    return utility_datetime::SecondsOf1Hour()*m_hour +
           utility_datetime::SecondsOf1Minute()*m_minute +
           m_second;
}

/*!
 *  @brief  ������œ���
 */
std::string HHMMSS::to_string() const
{
    const std::string zero("0");
    std::string ret;

    if (m_hour < 10) {
        ret += zero;
    }
    ret += std::to_string(m_hour);
    if (m_minute < 10) {
        ret += zero;
    }
    ret += std::to_string(m_minute);
    if (m_second < 10) {
        ret += zero;
    }
    ret += std::to_string(m_second);

    return ret;
}
/*!
 *  @brief  ':'��؂�̕�����œ���
 */
std::string HHMMSS::to_delim_string() const
{
    const std::string zero("0");
    const std::string delim(DELIM);
    std::string ret;

    if (m_hour < 10) {
        ret += zero;
    }
    ret += std::to_string(m_hour) + delim;
    if (m_minute < 10) {
        ret += zero;
    }
    ret += std::to_string(m_minute) + delim;
    if (m_second < 10) {
        ret += zero;
    }
    ret += std::to_string(m_second);

    return ret;
}

} // namespace garnet
