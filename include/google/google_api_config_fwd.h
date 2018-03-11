/*!
 *  @file   google_api_config_fwd.h
 *  @brief  [common]GoogleAPIê›íËforward
 *  @date   2018/03/10
 */
#pragma once

#include <memory>

namespace garnet
{
class GoogleCalendarAPIConfig;
typedef std::shared_ptr<const GoogleCalendarAPIConfig> GoogleCalendarAPIConfigPtr;
typedef std::weak_ptr<const GoogleCalendarAPIConfig> GoogleCalendarAPIConfigRef;

} // namespace garnet
