/*!
 *  @file   twitter_session.h
 *  @brief  [common]twitterAPI�Z�b�V����
 *  @date   2017/12/31
 *  @note   C++ REST SDK, boost. utility_datetime �Ɉˑ����Ă���
 */
#pragma once

#include <string>
#include <memory>

namespace garnet
{

namespace twitter
{

/*!
 *  @brief  ���s�R�[�h������𓾂�
 */
const std::wstring GetNewlineString();

} //  namespace twitter

/*!
 *  @brief  AccessToken/Secret���������Ă���author��p�Z�b�V����
 *  @note   ���[�U�Ƀu���E�U�F�؂𑣂�token���擾����V�[�P���X�͊܂܂�Ȃ�
 *  @note   token�͐ݒ�t�@�C��(twitter_session.ini)����擾
 */
class TwitterSessionForAuthor
{
public:
    TwitterSessionForAuthor();
    ~TwitterSessionForAuthor();

    /*!
     *  @brief  �c�C�[�g����(�������A)
     *  @param  date    ����������(ASCII)
     *  @param  src     �c�C�[�g������(utf-16)
     *  @retval ���M����
     *  @note   280�����ȏ゠������e��
     *  @note   ASCII�R�[�h�̃`�F�b�N�͂��ĂȂ��̂ŁAtwitter���Œe����邱�Ƃ�����
     */
    bool Tweet(const std::wstring& date, const std::wstring& src);
    bool Tweet(const std::wstring& src);

private:
    TwitterSessionForAuthor(const TwitterSessionForAuthor&);
    TwitterSessionForAuthor& operator= (const TwitterSessionForAuthor&);

    class PIMPL;
    std::unique_ptr<PIMPL> m_pImpl;
};

} // namespace garnet
