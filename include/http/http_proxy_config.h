/*!
 *  @file   http_proxy_config.h
 *  @brief  [common]proxy�ݒ�
 *  @date   2018/05/13
 *  @note   lua �Ɉˑ����Ă�/namespace��cpprestsdk����
 */
#pragma once

#include <string>
#include <vector>

namespace web
{
namespace http
{
/*!
 *  @brief  proxy�ݒ�
 *  @note   lua�t�@�C����ǂݍ���ŊO���ɒ�
 */
class proxy_config
{
public:
    /*!
     */
    proxy_config();
    /*!
     *  @param  lua�t�@�C����
     */
    proxy_config(const std::string& lua_file_name);

    /*!
     *  @brief  proxy���X�g�𓾂�
     */
    const std::vector<std::string>& get_proxy_list() const { return m_proxy_list; }
    /*!
     *  @brief  �x���b���𓾂�
     */
    int32_t get_sleep_second() const { return m_sleep_second; }
    /*!
     *  @brief  �A���G���[����𓾂�
     */
    int32_t get_max_error_count() const { return m_max_error_count; }

private:
    proxy_config(const proxy_config&);
    proxy_config(proxy_config&&);
    proxy_config& operator= (const proxy_config&);

    /*!
     *  @brief  ������
     *  @param  lua�t�@�C����
     */
    void initialize(const std::string& lua_file_name);

    //
    std::vector<std::string> m_proxy_list;
    int32_t m_sleep_second;
    int32_t m_max_error_count;
};

} // namespace http
} // namespace web
