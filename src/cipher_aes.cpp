/*!
 *  @file   cipher_aes.cpp
 *  @brief  [common]AES�ňÍ���������N���X
 *  @date   2017/12/18
 */
#include "cipher_aes.h"

#include "utility_string.h"

#include "aes.h"
#include "dh.h"
#include "modes.h"
#include "osrng.h"
#include <codecvt>

namespace garnet
{

class CipherAES_string::PIMPL
{
private:
    std::string m_key;  //! �Í���
    std::string m_iv;   //! InitialVector
    
    //! �t�B���^�������ɓn���Í����I�u�W�F�N�g(���[�J���͂܂������Ȃ̂�)
    CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption m_encryption;
    //! �Í���������
    std::string m_enc_text; 
    //! �Í����t�B���^
    std::unique_ptr<CryptoPP::StreamTransformationFilter> m_pEncryptFilter;

    PIMPL(const PIMPL&);
    PIMPL& operator= (const PIMPL&);

public:
    PIMPL()
    : m_key()
    , m_iv()
    , m_pEncryptFilter()
    {
    }

    /*!
     *  @brief  ��������Í�������
     *  @param  rnd_gen ����������
     *  @param  in_str  �Í������镶����
     *  @return true    ����
     */
    bool Encrypt(RandomGenerator& rnd_gen, const std::string& src)
    {
        if (!m_key.empty() || m_pEncryptFilter != nullptr) {
            // �Í����ς�
            return false;
        }

        utility_string::GetRandomString(rnd_gen, CryptoPP::AES::DEFAULT_KEYLENGTH, m_key);
        utility_string::GetRandomString(rnd_gen, CryptoPP::AES::BLOCKSIZE, m_iv);

        // �Í����̂��߂̕ϊ��t�B���^�̍쐬
        m_encryption.SetKeyWithIV(reinterpret_cast<const byte*>(m_key.c_str()), m_key.length(),
                                  reinterpret_cast<const byte*>(m_iv.c_str()), m_iv.length());
        m_pEncryptFilter.reset(new CryptoPP::StreamTransformationFilter(m_encryption, new CryptoPP::StringSink(m_enc_text)));

        // �Í���
        m_pEncryptFilter->Put(reinterpret_cast<const byte*>(src.c_str()), src.size());
        m_pEncryptFilter->MessageEnd();

        return true;
    }

    /*!
     *  @brief  ������𕜍�����
     *  @param[out] dst     ��������������̊i�[��
     *  @return     true    ����
     */
    bool Decrypt(std::string& dst) const
    {
        if (m_key.empty() || nullptr == m_pEncryptFilter) {
            // �Í�������ĂȂ�
            return false;
        }

        // �������I�u�W�F�N�g�̍쐬
        CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption dec;
        dec.SetKeyWithIV(reinterpret_cast<const byte*>(m_key.c_str()), m_key.length(),
                         reinterpret_cast<const byte*>(m_iv.c_str()), m_iv.length());
        // �������̂��߂̕ϊ��t�B���^�̍쐬
        CryptoPP::StreamTransformationFilter decFilter(dec, new CryptoPP::StringSink(dst));
        decFilter.Put(reinterpret_cast<const byte*>(m_enc_text.c_str()), m_enc_text.size());
        decFilter.MessageEnd();
        
        return true;
    }
};

/*!
 *  @brief
 */
CipherAES_string::CipherAES_string()
: m_pImpl(new PIMPL())
{
}

/*!
 *  @brief
 */
CipherAES_string::~CipherAES_string()
{
}

/*!
 *  @brief  ��������Í�������
 *  @param  rnd_gen ����������
 *  @param  src     �Í������镶����
 */
bool CipherAES_string::Encrypt(RandomGenerator& rnd_gen, const std::string& src)
{
    return m_pImpl->Encrypt(rnd_gen, src);
}
/*!
 *  @brief  ������𕜍�����
 *  @param[out] dst     ��������������̊i�[��
 */
bool CipherAES_string::Decrypt(std::string& dst) const
{
    return m_pImpl->Decrypt(dst);
}
/*!
 *  @brief  ������𕜍�����(wstring��)
 *  @param[out] dst     ��������������̊i�[��
 */
bool CipherAES_string::Decrypt(std::wstring& dst) const
{
    std::string str_work;
    bool result = m_pImpl->Decrypt(str_work);
    if (result) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utfconv;
        dst = std::move(utfconv.from_bytes(str_work));
    }
    return result;
}

} // namespace garnet
