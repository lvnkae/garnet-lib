/*!
 *  @file   cipher_aes.h
 *  @brief  [common]AES�ňÍ���������N���X
 *  @date   2017/12/18
 *  @note   Crypto++, RandomGenerator, utility_string �Ɉˑ����Ă���
 */
#pragma once

#include <string>
#include <memory>

namespace garnet
{

class RandomGenerator;

/*!
 *  @brief  �������AES�ňÍ������ĕێ�����N���X
 *  @note   1������ɂ�1�C���X�^���X
 *  @note    �Í�����1�C���X�^���X��1�񂾂�
 *  @note    ����(���̕�������o��)�͉��x�ł�OK
 *  @note   �Í�����iv�͒��ŏ���ɍ��(�Q�ƕs��)
 */
class CipherAES_string
{
public:
    CipherAES_string();
    ~CipherAES_string();

    /*!
     *  @brief  ��������Í�������
     *  @param  rnd_gen ����������
     *  @param  src     �Í������镶����(utf-8)
     *  @retval true    ����
     */
    bool Encrypt(RandomGenerator& rnd_gen, const std::string& src);
    /*!
     *  @brief  ������𕜍�����
     *  @param[out] dst     ��������������̊i�[��(utf-8)
     *  @retval     true    ����
     */
    bool Decrypt(std::string& dst) const;
    /*!
     *  @brief  ������𕜍�����(wstring��)
     *  @param[out] dst     ��������������̊i�[��(utf-16)
     *  @retval     true    ����
     */
    bool Decrypt(std::wstring& dst) const;


private:
    CipherAES_string(const CipherAES_string&);
    CipherAES_string(CipherAES_string&&);
    CipherAES_string& operator= (const CipherAES_string&);

    class PIMPL;
    std::unique_ptr<PIMPL> m_pImpl;
};

} // namespace garnet
