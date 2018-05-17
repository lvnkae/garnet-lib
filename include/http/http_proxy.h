/*!
 *  @file   http_proxy_magazine.h
 *  @brief  [common]proxy
 *  @date   2018/05/13
 *  @note   cpprestsdk に依存(namespaceも準拠する)
 */
#pragma once

#include "http/http_proxy_config_fwd.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace web
{
namespace http
{

namespace client { class http_client_config; }

/*!
 *  @brief  proxyステータス
 */
struct proxy_status
{
    //! address:port
    std::wstring m_address_port;
    //! 最終使用時刻(tick)
    int64_t m_last_use_tick;
    //! エラーカウンタ
    int32_t m_error_cnt;

    proxy_status()
    : m_address_port()
    , m_last_use_tick(0)
    , m_error_cnt(0)
    {}
    proxy_status(std::wstring&& address_port)
    : m_address_port(address_port)
    , m_last_use_tick(0)
    , m_error_cnt(0)
    {}
};

/*!
 *  @brief  cookie_magazineクラス
 *  @note   複数のproxyを入れた弾倉
 */
class proxy_magazine
{
private:
    //! proxy弾倉
    std::vector< proxy_status> m_magazine;
    //! 現在使用中のproxy
    std::wstring m_current;
    //! 休眠時間
    int64_t m_sleep_tick;
    //! 連続エラー上限
    int32_t m_max_error_count;

public:
    /*!
     */
    proxy_magazine();
    /*!
     *  @param  config  proxy設定
     */
    proxy_magazine(const proxy_config_ref& config);
    /*!
     */
    ~proxy_magazine(){}

    /*!
     *  @brief  追加登録
     *  @param  proxy   proxy文字列(address:port)
     */
    void entry(const std::string& proxy);

    /*!
     *  @brief  定期更新
     *  @param  tick    経過時間[ミリ秒]
     *  @note   状況に応じたproxyの切り替え等
     */
    void update(int64_t tick);

    /*!
     *  @brief  現在使用中のproxyを得る
     */
    std::wstring get_current() const { return m_current; }
    /*!
     *  @brief  現在使用中のproxyを外す
     */
    void release_current() { m_current.swap(std::wstring()); }

    /*!
     *  @brief  最終使用時刻更新
     *  @param  proxy   proxy文字列(address:port)
     *  @param  tick    最終使用時刻(tick)[ミリ秒]
     */
    void update_tick(const std::wstring& proxy, int64_t tick);
    /*!
     *  @brief  エラーカウンタを増やす
     *  @param  proxy   proxy文字列(address:port)
     */
    void inc_error_count(const std::wstring& proxy);
    /*!
     *  @brief  エラーカウンタリセット
     *  @param  proxy   proxy文字列(address:port)
     */
    void reset_error_count(const std::wstring& proxy);

private:
    proxy_magazine(const proxy_config&);
    proxy_magazine(proxy_config&&);
    proxy_magazine& operator= (const proxy_config&);
};

} // namespace http
} // namespace cookie
