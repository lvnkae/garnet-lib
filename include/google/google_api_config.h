/*!
 *  @file   google_api_config.h
 *  @brief  [common]GoogleAPI設定
 *  @date   2018/03/10
 *  @note   boost に依存してる
 */
#pragma once

#include <string>

namespace garnet
{
/*!
 *  @brief  GoogleCalendarAPI設定
 *  @note   iniファイルを読み込んで外部に提供
 */
class GoogleCalendarAPIConfig
{
public:
    /*!
     *  @brief  空生成
     */
    GoogleCalendarAPIConfig();
    /*!
     *  @param  iniファイル名
     */
    GoogleCalendarAPIConfig(const std::string& ini_file_name);

    /*!
     *  @brief  APIKeyを得る
     */
    const std::string& GetAPIKey() const { return m_api_key; }
    /*!
     *  @brief  carendarIdを得る
     */
    const std::string& GetCarendarID() const { return m_calendar_id; }

private:
    GoogleCalendarAPIConfig(const GoogleCalendarAPIConfig&);
    GoogleCalendarAPIConfig(GoogleCalendarAPIConfig&&);
    GoogleCalendarAPIConfig& operator= (const GoogleCalendarAPIConfig&);

    /*!
     *  @brief  初期化
     *  @param  iniファイル名
     */
    void initialize(const std::string& ini_file_name);

    std::string m_api_key;
    std::string m_calendar_id;
};

} // namespace garnet
