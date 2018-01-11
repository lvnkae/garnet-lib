/*!
 *  @file   utility_http.h
 *  @brief  [common]HTTP�֘AUtility
 *  @date   2017/12/19
 *  @note   C++REST, utility_string �Ɉˑ����Ă���
 */
#pragma once

#include <string>

namespace web { namespace http { class http_request; } }
namespace web { namespace http { class http_headers; } }
namespace web { namespace http { class cookies_group; } }

namespace garnet
{
namespace utility_http
{

/*!
 *  @brief  ���M���N�G�X�g�̃w�b�_�[���ʐݒ�(�ŏ��\��)
 *  @param[out] request ���M���N�G�X�g(����Ώ�)
 *  @note   Content-Length�̓Z�b�g����Ȃ�(SetFormData�őΉ�)
 */
void SetHttpCommonHeaderSimple(web::http::http_request& request);
/*!
 *  @brief  ���M���N�G�X�g�̃w�b�_�[���ʐݒ�(keep-alive��)
 *  @param[in]  url         ���M��URL
 *  @param[in]  cookies_gr  �N�b�L�[�Q
 *  @param[in]  referer     ���t�@��(empty�Ȃ�Z�b�g���Ȃ�)
 *  @param[out] request     ���M���N�G�X�g(����Ώ�)
 *  @note   Content-Length�̓Z�b�g����Ȃ�(SetFormData�őΉ�)
 */
void SetHttpCommonHeaderKeepAlive(const std::wstring& url, const web::http::cookies_group& cookies_gr, const std::wstring& referer, web::http::http_request& request);
/*!
 *  @brief  ���M���N�G�X�g��form data��ݒ肷��
 *  @param[in]  src     �ݒ肷�镶����(form data)
 *  @param[out] request ���M���N�G�X�g
 *  @note   Content-Length�̎Z�o�y�уZ�b�g���s��
 */
void SetFormData(const std::wstring& src, web::http::http_request& request);

/*!
 *  @brief  form data�ɗv�f��ǉ�����
 *  @param[in]  param   form data�p�����[�^
 *  @param[out] dst     �i�[��
 */
struct sFormDataParam
{
    const std::wstring m_name;
    const std::wstring m_value;
};
void AddFormDataParamToString(const sFormDataParam& param, std::wstring& dst);
/*!
 *  @brief  form data�ɗv�f��ǉ�����
 *  @param[in]  param_name  �p�����[�^��
 *  @param[in]  param_value �p�����[�^�l
 *  @param[out] dst         �i�[��
 */
void AddFormDataParamToString(const std::wstring& param_name, const std::wstring& param_value, std::wstring& dst);

/*!
 *  @brief  URL��item��ǉ�����
 *  @param[in]  iname   item-name
 *  @param[in]  ivalue  item-value
 *  @param[out] url     ������
 *  @note   URL�Ɛ擪item��'?'�Aitem�Ԃ�'&'�Ōq��
 *  @note   item-name��value��'='�Ōq��
 */
void AddItemToURL(const std::wstring& iname, const std::wstring& ivalue, std::wstring& url);

/*!
 *  @brief  URL����h���C���𓾂�(�؂�o��)
 *  @param[in]  url     URL
 *  @param[in]  domain  �h���C��(�i�[��)
 *  @note  �h���C���̑啶��/�������͋�ʂ��Ȃ�
 *  @note  ���{��h���C���͍l�����ĂȂ�(����s��)
 */
void GetDomainFromURL(const std::string& url, std::string& domain);
void GetDomainFromURL(const std::wstring& urlT, std::string& domain);

} // namespace utility_http
} // namespace garnet
