/*!
 *  @file   http_proxy_config.h
 *  @brief  [common]proxy設定
 *  @date   2018/05/13
 *  @note   lua に依存してる/namespaceはcpprestsdk準拠
 */
#pragma once

#include <string>
#include <vector>

namespace web
{
namespace http
{
/*!
 *  @brief  proxy設定
 *  @note   luaファイルを読み込んで外部に提供
 */
class proxy_config
{
public:
    /*!
     */
    proxy_config();
    /*!
     *  @param  luaファイル名
     */
    proxy_config(const std::string& lua_file_name);

    /*!
     *  @brief  proxyリストを得る
     */
    const std::vector<std::string>& get_proxy_list() const { return m_proxy_list; }
    /*!
     *  @brief  休眠秒数を得る
     */
    int32_t get_sleep_second() const { return m_sleep_second; }
    /*!
     *  @brief  連続エラー上限を得る
     */
    int32_t get_max_error_count() const { return m_max_error_count; }

private:
    proxy_config(const proxy_config&);
    proxy_config(proxy_config&&);
    proxy_config& operator= (const proxy_config&);

    /*!
     *  @brief  初期化
     *  @param  luaファイル名
     */
    void initialize(const std::string& lua_file_name);

    //
    std::vector<std::string> m_proxy_list;
    int32_t m_sleep_second;
    int32_t m_max_error_count;
};

} // namespace http
} // namespace web
