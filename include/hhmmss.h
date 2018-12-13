/*!
 *  @file   hhmmss.h
 *  @brief  [common]�����b�����̍ŏ����ԍ\��
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
     *  @brief  "HH:MM:SS"�`���̔N���������񂩂琶��
     *  @param  src �����b������
     */
    static HHMMSS Create(const std::string& src);

    /*!
     *  @brief  ������œ���
     */
    std::string to_string() const;
    /*!
     *  @brief  ':'��؂�̕�����œ���
     */
    std::string to_delim_string() const;

    /*!
     *  @brief  00:00:00����̌o�ߕb���𓾂�
     */
    int32_t GetPastSecond() const;
    /*!
     *  @brief  �����Z
     *  @return this-right��b�P�ʂŕԂ�
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
