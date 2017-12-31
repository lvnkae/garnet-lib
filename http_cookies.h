/*!
 *  @file   http_cookies.h
 *  @brief  [common]cookie
 *  @date   2017/12/25
 *  @note   C++REST �Ɉˑ�(namespace����������)
 */
#pragma once

#include <string>
#include <unordered_map>

namespace web
{
namespace http
{

class http_headers;

/*!
 *  @brief  cookie�N���X
 *  @note   1�h���C����
 */
class cookies
{
private:
    //! �L�[�ƃp�����[�^�̃Z�b�g
    std::unordered_map<std::wstring, std::wstring> m_contents;

public:
    /*!
     *  @brief  ��M�w�b�_�̎���cookie�����o���ăZ�b�g
     *  @param  headers ResponseHeaders
     */
    void Set(const web::http::http_headers& headers);
    /*!
     *  @brief  �A��������̕�����Ƃ��Ď擾����
     *  @param[out] dst RequestHeaders��cookie�ɓ���镶����(�i�[��)
     *  @note   key��param��'='�Ōq����obj��';'�ŋ�؂��ĘA��
     */
    void Get(std::wstring& dst) const;

    cookies()
    : m_contents()
    {}
    ~cookies(){}
};

/*!
 *  @brief  cookie_group�N���X
 *  @note   �h���C�����L�[�ɕ�����cookie�𑩂˂�����
 *  @note   ���p�p���L���h���C���̂ݑΉ�(����ȊO�͐��K�����Ɏ̂Ă���)
 */
class cookies_group
{
private:
    //! �h���C���L�[��cookies�̃Z�b�g
    std::unordered_map<std::string, cookies> m_cookies;

public:
    /*!
     *  @brief  http_headers����cookie�����o���ăZ�b�g
     *  @param  headers ResponseHeaders
     *  @param  url     ���M��URL(headers���ێ����Ă���Ă�Ίy�������̂Ɂc)
     */
    void Set(const web::http::http_headers& headers, const std::string& url);
    void Set(const web::http::http_headers& headers, const std::wstring& urlT);
    /*!
     *  @brief  cookie��A��������̕�����Ƃ��Ď擾����
     *  @param[in]  url ���M��URL
     *  @param[out] dst �i�[��
     *  @note   RequestHeaders�ɐݒ肷��cookie������𓾂�
     */
    void Get(const std::string& url, std::wstring& dst) const;
    void Get(const std::wstring& url, std::wstring& dst) const;

    cookies_group()
    : m_cookies()
    {}
    ~cookies_group(){}
};

} // namespace http
} // namespace cookie
