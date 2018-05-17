/*!
 *  @file   utility_string.h
 *  @brief  [common]�����񑀍�Utility
 *  @date   2017/12/18
 *  @note   RandomGenerator, boost::algorithm �Ɉˑ����Ă���
 */
#pragma once

#include <string>
#include <unordered_set>
#include <vector>

namespace garnet
{
class RandomGenerator;

namespace utility_string
{

/*!
 *  @brief  �����_����ASCII������𓾂�
 *  @param[in]  rnd_gen ����������
 *  @param[in]  len     ������
 *  @param[out] o_str   �i�[��
 *  @note   ASCII�̐����E�p��(�召)�̕�����𓾂�B�L���͊܂܂Ȃ��B
 */
void GetRandomString(RandomGenerator& rnd_gen, size_t len, std::string& o_string);

/*!
 *  @brief  string��wstring�R�s�[
 *  @param  src ���͕�����
 *  @note   �����R�[�h�͈�؍l�����Ȃ������̃R�s�[
 */
std::wstring ToWstring(const std::string& src);

/*!
 *  @brief  �啶�����������ϊ�
 *  @param[in]  src
 *  @param[out] dst
 *  @note   �}���`�o�C�g�����͐؂�̂Ă�
 *  @note   UTF-8�͐��������ʂ������Ȃ�(�}���`�o�C�g�������������Ă�Ƃ��̈ꕔ���A���t�@�x�b�g�ɉ�����)
 */
void ToLower(const std::wstring& src, std::string& dst);
void ToLower(const std::string& src, std::string& dst);

/*!
 *  @brief  �����񕪊�
 *  @param[in]  src
 *  @param[in]  delim   ��������(�f���~�^)
 *  @param[out] dst     ���ʊi�[��
 */
void Split(const std::string& src, const std::string& delim, std::vector<std::string>& dst);
void Split(const std::string& src, const std::string& delim, std::unordered_set<std::string>& dst);

/*!
 *  @brief  �{���x���������������_��n�ʂ܂ŕ�����ɕϊ�
 *  @param  src     ���͏����l
 *  @param  order   �c�������_����
 *  @return ������
 *  @note   std::to_wstring/std::to_string���ƌ����w�肪�ł��Ȃ��̂�
 */
std::wstring ToWstringOrder(float64 src, uint32_t order);
std::string ToStringOrder(float64 src, uint32_t order);
/*!
 *  @brief  �铽������ID(���l)����N������������ɕϊ�
 *  @param  src     ID
 *  @param  order   �\�����鉺����
 *  @return ������
 *  @note   ��ʌ���*�Ŗ��߂�
 */
std::wstring ToSecretIDOrder(int32_t src, uint32_t order);

} // namespace utility_string
} // namespace garnet
