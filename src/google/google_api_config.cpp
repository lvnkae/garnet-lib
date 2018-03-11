/*!
 *  @file   google_api_config.h
 *  @brief  [common]GoogleAPI�ݒ�
 *  @date   2018/03/10
 */
#include "google/google_api_config.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include "boost/optional.hpp"

#include <fstream>

namespace garnet
{

/*!
 *  @brief  �󐶐�
 */
GoogleCalendarAPIConfig::GoogleCalendarAPIConfig()
: m_api_key()
, m_calendar_id()
{
}

/*!
 *  @param  ini�t�@�C����
 */
GoogleCalendarAPIConfig::GoogleCalendarAPIConfig(const std::string& ini_file_name)
: m_api_key()
, m_calendar_id()
{
    initialize(ini_file_name);
}

/*!
 *  @brief  ������
 *  @param  ini�t�@�C����
 */
void GoogleCalendarAPIConfig::initialize(const std::string& ini_file_name)
{
    std::ifstream ifs(ini_file_name);
    if (!ifs.is_open()) {
        return; // ini�t�@�C�����Ȃ�
    }
    boost::property_tree::ptree pt;
    boost::property_tree::read_ini(ini_file_name, pt);

    boost::optional<std::string> api_key
        = pt.get_optional<std::string>("Calendar.APIKey");
    m_api_key = api_key.get();
    boost::optional<std::string> calendar_id
        = pt.get_optional<std::string>("Calendar.calendarId");
    m_calendar_id = calendar_id.get();
}

} // namespace garnet
