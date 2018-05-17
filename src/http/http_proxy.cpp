/*!
 *  @file   http_proxy.cpp
 *  @brief  [common]proxy
 *  @date   2018/05/13
 */
#include "http/http_proxy.h"

#include "http/http_proxy_config.h"
#include "utility/utility_datetime.h"
#include "utility/utility_http.h"
#include "utility/utility_string.h"

#include "cpprest/base_uri.h"
#include "cpprest/http_headers.h"

namespace web
{
namespace http
{

/*!
 */
proxy_magazine::proxy_magazine()
: m_magazine()
, m_current()
, m_sleep_tick(0)
, m_max_error_count(0)
{
}
/*!
 *  @param  config  proxy設定
 */
proxy_magazine::proxy_magazine(const proxy_config_ref& config)
: m_magazine()
, m_current()
, m_sleep_tick(0)
, m_max_error_count(0)
{
    const proxy_config_ptr p = config.lock();
    if (nullptr != p) {
        const std::vector<std::string>& proxy_list = p->get_proxy_list();
        m_magazine.reserve(proxy_list.size());
        for (const auto& proxy: proxy_list) {
            entry(proxy);
        }
        m_sleep_tick = garnet::utility_datetime::ToMiliSecondsFromSecond(p->get_sleep_second());
        m_max_error_count = p->get_max_error_count();
    }
}

/*!
 *  @brief  追加登録
 *  @param  proxy   proxy文字列(address:port)
 */
void proxy_magazine::entry(const std::string& proxy)
{
    m_magazine.emplace_back(garnet::utility_string::ToWstring(proxy));
}

/*!
 *  @brief  定期更新
 *  @param  tick    経過時間[ミリ秒]
 */
void proxy_magazine::update(int64_t tick)
{
    // エラー上限に達したproxyをreleaseする
    for (auto& mag: m_magazine) {
        if (mag.m_error_cnt >= m_max_error_count) {
            if (m_current == mag.m_address_port) {
                release_current();
                mag.m_error_cnt = 0;
            }
        }
    }
    // currentが空だったらあてがう
    if (m_current.empty()) {
        for (const auto& mag: m_magazine) {
            const int64_t last = mag.m_last_use_tick;
            if (0 == last) {
                // 未使用なので使ってよし
                m_current = mag.m_address_port;
                break;
            }
            const int64_t diff = tick - mag.m_last_use_tick;
            if (diff >= m_sleep_tick) {
                // 指定時間以上休んでたので使ってよし
                m_current = mag.m_address_port;
                break;
            }
        }
    }
}

/*!
 *  @brief  ステータス更新
 *  @param  proxy   proxy文字列(address:port)
 *  @param  tick    最終使用時刻(tick)[ミリ秒]
 */
void proxy_magazine::update_tick(const std::wstring& proxy, int64_t tick)
{
    auto it = std::find_if(m_magazine.begin(),
                           m_magazine.end(),
                           [&proxy](const auto& mag) {
        return (proxy == mag.m_address_port);
    });
    if (it == m_magazine.end()) {
        return;
    }
    it->m_last_use_tick = tick;
}

/*!
 *  @brief  エラーカウンタを増やす
 *  @param  proxy   proxy文字列(address:port)
 */
void proxy_magazine::inc_error_count(const std::wstring& proxy)
{
    auto it = std::find_if(m_magazine.begin(),
                           m_magazine.end(),
                           [&proxy](const auto& mag) {
        return (proxy == mag.m_address_port);
    });
    if (it == m_magazine.end()) {
        return;
    }
    it->m_error_cnt++;
}
/*!
 *  @brief  エラーカウンタリセット
 *  @param  proxy   proxy文字列(address:port)
 */
void proxy_magazine::reset_error_count(const std::wstring& proxy)
{
    auto it = std::find_if(m_magazine.begin(),
                           m_magazine.end(),
                           [&proxy](const auto& mag) {
        return (proxy == mag.m_address_port);
    });
    if (it == m_magazine.end()) {
        return;
    }
    it->m_error_cnt = 0;
}

} // namespace http
} // namespace web
