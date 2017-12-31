/*!
 *  @file   utility_http.cpp
 *  @brief  [common]HTTP�֘AUtility
 *  @date   2017/12/19
 */
#include "utility_http.h"

#include "http_cookies.h"
#include "utility_string.h"

#include "boost/algorithm/string.hpp"
#include "cpprest/http_headers.h"
#include "cpprest/http_msg.h"
#include <codecvt>
#include <iostream>

namespace utility
{

/*!
 *  @brief  ���M���N�G�X�g�̃w�b�_�[���ʐݒ�(�ŏ��\��)
 *  @param[out] request ���M���N�G�X�g(����Ώ�)
 */
void SetHttpCommonHeaderSimple(web::http::http_request& request)
{
    request.headers().add(L"Accept", L"text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    request.headers().add(L"Content-Type", L"application/x-www-form-urlencoded; charset=UTF-8");
    request.headers().add(L"User-Agent", L"Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36");
}
/*!
 *  @brief  ���M���N�G�X�g�̃w�b�_�[���ʐݒ�(keep-alive��)
 *  @param[in]  url         ���M��URL
 *  @param[in]  cookies_gr  �N�b�L�[�Q
 *  @param[in]  referer     ���t�@��(empty�Ȃ�Z�b�g���Ȃ�)
 *  @param[out] request     ���M���N�G�X�g(����Ώ�)
 */
void SetHttpCommonHeaderKeepAlive(const std::wstring& url, const web::http::cookies_group& cookies_gr, const std::wstring& referer, web::http::http_request& request)
{
    SetHttpCommonHeaderSimple(request);
    request.headers().add(L"Connection", L"keep-alive");
    std::wstring cookie_str;
    cookies_gr.Get(url, cookie_str);
    if (!cookie_str.empty()) {
        request.headers().add(L"Cookie", cookie_str);
    }
    if (!referer.empty()) {
        request.headers().add(L"Referer", referer);
    }
}

/*!
 *  @brief  ���M���N�G�X�g��form data��ݒ肷��
 *  @param[in]  src     �ݒ肷�镶����(2byte������)
 *  @param[out] request ���M���N�G�X�g
 */
void SetFormData(const std::wstring& src, web::http::http_request& request)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
    std::string body_u8(cv.to_bytes(src));
    const size_t body_size = body_u8.length();
    request.headers().add(L"Content-Length", std::to_wstring(body_size));
    request.set_body(src);
}

/*!
 *  @brief  form data�ɗv�f��ǉ�����
 *  @param[in]  param_name  �p�����[�^��
 *  @param[in]  param_value �p�����[�^�l
 *  @param[out] dst         �i�[��
 */
void AddFormDataParamToString(const std::wstring& param_name, const std::wstring& param_value, std::wstring& dst)
{
    if (dst.empty()) {
        dst = param_name + L"=" + param_value;
    } else {
        dst += L"&" + param_name + L"=" + param_value;
    }
}
/*!
 *  @brief  form data�ɗv�f��ǉ�����
 *  @param[in]  param   form data�p�����[�^
 *  @param[out] dst     �i�[��
 */
void AddFormDataParamToString(const sFormDataParam& param, std::wstring& dst)
{
    AddFormDataParamToString(param.m_name, param.m_value, dst);
}

/*!
 *  @brief  URL��item��ǉ�����
 *  @param[in]  iname   item-name
 *  @param[in]  ivalue  item-value
 *  @param[out] url     ������
 *  @note   URL�Ɛ擪item��'?'�Aitem�Ԃ�'&'�Ōq��
 *  @note   item-name��value��'='�Ōq��
 */
void AddItemToURL(const std::wstring& iname, const std::wstring& ivalue, std::wstring& url)
{
    if (std::wstring::npos == url.find(L'?')) {
        url += L'?' + iname + L"=" + ivalue;
    } else {
        url += L'&' + iname + L"=" + ivalue;
    }
}

/*!
 *  @brief  URL����h���C���𓾂�(�؂�o��)
 *  @param[in]  url     URL
 *  @param[in]  domain  �h���C��(�i�[��)
 *  @note  �h���C���̑啶��/�������͋�ʂ��Ȃ�
 *  @note  ���{��h���C���͍l�����ĂȂ�(����s��)
 */
template<typename T>
void GetDomainFromURCoreL(const T& url, std::string& domain)
{
    utility::ToLower(url, domain);
    std::vector<std::string> url_split;
    boost::algorithm::split(url_split, domain, boost::is_any_of("/"));
    const size_t len = url_split.size();
    if (len < 2) {
        domain.swap(std::string());
    } else {
        domain.swap(url_split[2]); // [http:][][domain][etc]�ɕ��������
    }
}
void GetDomainFromURL(const std::string& url, std::string& domain) { GetDomainFromURCoreL(url, domain); }
void GetDomainFromURL(const std::wstring& urlT, std::string& domain) { GetDomainFromURCoreL(urlT, domain); }

} // namespace utility
