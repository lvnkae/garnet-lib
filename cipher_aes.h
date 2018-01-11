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
 *  @note   �E�g���̂ĂȂ̂�1�N���X��1�񂵂��Í����ł��Ȃ�(�����͉��x�ł�OK)
 *  @note   �E�Í�����iv�͒��ŏ���ɍ��
 */
class CipherAES
{
public:
    CipherAES();
    ~CipherAES();

    /*!
     *  @brief  ��������Í�������
     *  @param  rnd_gen ����������
     *  @param  src     �Í������镶����
     *  @retval true    ����
     */
    bool Encrypt(RandomGenerator& rnd_gen, const std::string& src);
    /*!
     *  @brief  ������𕜍�����
     *  @param[out] dst     ��������������̊i�[��
     *  @retval     true    ����
     */
    bool Decrypt(std::string& dst) const;
    /*!
     *  @brief  ������𕜍�����(wstring��)
     *  @param[out] dst     ��������������̊i�[��
     *  @retval     true    ����
     */
    bool Decrypt(std::wstring& dst) const;


private:
    CipherAES(const CipherAES&);
    CipherAES& operator= (const CipherAES&);

    class PIMPL;
    std::unique_ptr<PIMPL> m_pImpl;
};

} // namespace garnet
