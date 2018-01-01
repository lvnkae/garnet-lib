/*!
 *  @file   utility_datetime.h
 *  @brief  [common]�����֘AUtility
 *  @date   2017/12/19
 *  @note   boost::date_time �Ɉˑ����Ă���
 */
#pragma once

#include <string>
#include "boost/date_time/posix_time/posix_time.hpp"

namespace std { struct tm; }

namespace utility
{

/*!
 *  @brief  �j��
 *  @note   tm::wday�`��(days since Sunday - [0,6])
 */
enum WeekOfDay
{
    SUNDAY = 0,
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
};

/*!
 *  @brief  RFC1123�`���̓��������񂩂烍�[�J�������𓾂�
 *  @param  rfc1123 RFC1123�`������������
 *  @return boost���ԃC���^�[�t�F�[�X
 *  @note   "Fri, 03 Jan 2014 08:39:11 GMT"
 *  @note   http�w�b�_�ɓ����Ă遪�`���̂��
 *  @note   �����[�J��=GMT+09(JST)�ɌŒ�
 */
boost::posix_time::ptime ToLocalTimeFromRFC1123(const std::string& rfc1123);
boost::posix_time::ptime ToLocalTimeFromRFC1123(const std::wstring& rfc1123);
/*!
 *  @brief  �w��t�H�[�}�b�g�̓����������tm�ɕϊ�
 *  @param[in]  src     ����������
 *  @param[in]  format  ���`�� (%H:%M:%S �Ȃ�)
 *  @param[out] o_tm    �i�[��
 *  @note   ���[�J���ϊ��Ȃ�
 */
bool ToTimeFromString(const std::string& src, const std::string& format, std::tm& o_tm);
/*!
 *  @brief  ptime����tm�ɕϊ�
 *  @param[in]  src     ptime
 *  @param[out] o_tm    �i�[��
 *  @param[in]  tm::tm_isdst�͏��false(�T�}�[�^�C���l�����Ȃ�)
 */
void ToTimeFromBoostPosixTime(const boost::posix_time::ptime& src, std::tm& o_tm);

/*!
 *  @brief  �o�ߎ��Ԃ𓾂�
 *  @return tickCount(�~���b)
 *  @note   Wiinbase��GetTickCount���g��Ȃ��ėp����
 *  @note   �N�_���ǂ��Ȃ̂��͕s��(���ʂ̓V�X�e���N��)
 */
int64_t GetTickCountGeneral();

/*!
 *  @brief  src�ƃV�X�e�����[�J�����ԂƂ̍���b[��Βl]�œ���
 *  @param  src ��ׂ鎞��
 *  @return �b��
 */
uint32_t GetDiffSecondsFromLocalMachineTime(const std::tm& src);
/*!
 *  @brief  �V�X�e�����[�J�����Ԃ𕶎���œ���
 *  @param[in]  format  �擾�`�� (%H:%M:%S �Ȃ�)
 *  @return ������
 */
std::wstring GetLocalMachineTime(const std::wstring& format);

/*!
 *  @brief  �N���������b�ɔC�Ӄ~���b�𑫂�
 *  @param[in]  base_tm �x�[�X
 *  @param[in]  diff_ms �����~���b
 *  @param[out] o_now   �i�[��
 */
void AddTimeAndDiffMS(const std::tm& base_tm, int64_t diff_ms, std::tm& o_now);

/*!
 *  @brief  �w�����after_day���00:00�܂ł̎��Ԃ��~���b�œ���
 *  @param  pt          boost���ԃC���^�[�t�F�C�X
 *  @param  after_day   �����ォ(������1)
 *  @return �c�莞��(�~���b)
 *  @note   0����w��Ȃ���0��Ԃ�
 */
int64_t GetAfterDayLimitMS(const boost::posix_time::ptime& pt, int32_t after_day);
/*!
 *  @brief  �����~���b�œ���
 *  @param  minute  �C�ӎ���[��]
 *  @retval �C�ӎ���[�~���b]
 */
int64_t ToMiliSecondsFromMinute(int32_t minute);
/*!
 *  @brief  �b���~���b�œ���
 *  @param  second  �C�ӎ���[�b]
 *  @retval �C�ӎ���[�~���b]
 */
int64_t ToMiliSecondsFromSecond(int32_t second);


} // namespace utility
