/*!
 *  @file   google_api_config.h
 *  @brief  [common]GoogleAPI�ݒ�
 *  @date   2018/03/10
 *  @note   boost �Ɉˑ����Ă�
 */
#pragma once

#include <string>

namespace garnet
{
/*!
 *  @brief  GoogleCalendarAPI�ݒ�
 *  @note   ini�t�@�C����ǂݍ���ŊO���ɒ�
 */
class GoogleCalendarAPIConfig
{
public:
    /*!
     *  @brief  �󐶐�
     */
    GoogleCalendarAPIConfig();
    /*!
     *  @param  ini�t�@�C����
     */
    GoogleCalendarAPIConfig(const std::string& ini_file_name);

    /*!
     *  @brief  APIKey�𓾂�
     */
    const std::string& GetAPIKey() const { return m_api_key; }
    /*!
     *  @brief  carendarId�𓾂�
     */
    const std::string& GetCarendarID() const { return m_calendar_id; }

private:
    GoogleCalendarAPIConfig(const GoogleCalendarAPIConfig&);
    GoogleCalendarAPIConfig(GoogleCalendarAPIConfig&&);
    GoogleCalendarAPIConfig& operator= (const GoogleCalendarAPIConfig&);

    /*!
     *  @brief  ������
     *  @param  ini�t�@�C����
     */
    void initialize(const std::string& ini_file_name);

    std::string m_api_key;
    std::string m_calendar_id;
};

} // namespace garnet
