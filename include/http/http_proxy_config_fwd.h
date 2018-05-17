/*!
 *  @file   http_proxy_config.h
 *  @brief  [common]proxyê›íËforward
 *  @date   2018/05/13
 */
#pragma once

#include <memory>

namespace web
{
namespace http
{
class proxy_config;
typedef std::shared_ptr<const proxy_config> proxy_config_ptr;
typedef std::weak_ptr<const proxy_config> proxy_config_ref;

} // namespace http
} // namespace web
