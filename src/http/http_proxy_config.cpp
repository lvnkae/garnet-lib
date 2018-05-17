/*!
 *  @file   proxy_config.h
 *  @brief  [common]proxy設定
 *  @date   2018/05/13
 */
#include "http/http_proxy_config.h"

#include "lua_accessor.h"

#include <fstream>

namespace web
{
namespace http
{

/*!
 */
proxy_config::proxy_config()
: m_proxy_list()
{
}

/*!
 *  @param  luaファイル名
 */
proxy_config::proxy_config(const std::string& lua_file_name)
: m_proxy_list()
, m_sleep_second()
, m_max_error_count()
{
    initialize(lua_file_name);
}

/*!
 *  @brief  初期化
 *  @param  luaファイル名
 */
void proxy_config::initialize(const std::string& lua_file_name)
{
    std::ifstream ifs(lua_file_name);
    if (!ifs.is_open()) {
        return; // luaファイルがない
    }
    garnet::LuaAccessor m_lua_accessor;
    if (!m_lua_accessor.DoFile(lua_file_name)) {
        return;
    }
    const int32_t num_proxy = m_lua_accessor.OpenTable("Magazine");
    m_proxy_list.reserve(num_proxy);
    for (int32_t inx = 0; inx < num_proxy; inx++) {
        std::string proxy;
        if (m_lua_accessor.GetArrayParam(inx, proxy)) {
            m_proxy_list.emplace_back(std::move(proxy));
        }
    }
    m_lua_accessor.CloseTable();

    m_lua_accessor.GetGlobalParam("SleepSecond", m_sleep_second);
    m_lua_accessor.GetGlobalParam("MaxErrorCount", m_max_error_count);
}

} // namespace http
} // namespace web
