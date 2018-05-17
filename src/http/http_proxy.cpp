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
 *  @param  config  proxy�ݒ�
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
 *  @brief  �ǉ��o�^
 *  @param  proxy   proxy������(address:port)
 */
void proxy_magazine::entry(const std::string& proxy)
{
    m_magazine.emplace_back(garnet::utility_string::ToWstring(proxy));
}

/*!
 *  @brief  ����X�V
 *  @param  tick    �o�ߎ���[�~���b]
 */
void proxy_magazine::update(int64_t tick)
{
    // �G���[����ɒB����proxy��release����
    for (auto& mag: m_magazine) {
        if (mag.m_error_cnt >= m_max_error_count) {
            if (m_current == mag.m_address_port) {
                release_current();
                mag.m_error_cnt = 0;
            }
        }
    }
    // current���󂾂����炠�Ă���
    if (m_current.empty()) {
        for (const auto& mag: m_magazine) {
            const int64_t last = mag.m_last_use_tick;
            if (0 == last) {
                // ���g�p�Ȃ̂Ŏg���Ă悵
                m_current = mag.m_address_port;
                break;
            }
            const int64_t diff = tick - mag.m_last_use_tick;
            if (diff >= m_sleep_tick) {
                // �w�莞�Ԉȏ�x��ł��̂Ŏg���Ă悵
                m_current = mag.m_address_port;
                break;
            }
        }
    }
}

/*!
 *  @brief  �X�e�[�^�X�X�V
 *  @param  proxy   proxy������(address:port)
 *  @param  tick    �ŏI�g�p����(tick)[�~���b]
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
 *  @brief  �G���[�J�E���^�𑝂₷
 *  @param  proxy   proxy������(address:port)
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
 *  @brief  �G���[�J�E���^���Z�b�g
 *  @param  proxy   proxy������(address:port)
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
