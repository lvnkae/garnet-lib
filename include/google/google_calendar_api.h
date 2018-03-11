/*!
 *  @file   google_calendar_api.h
 *  @brief  [common]GoogleCalendarAPI
 *  @date   2018/03/10
 *  @note   cpprestsdk, boost に依存している
 */
#pragma once

#include "google_api_config_fwd.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace garnet
{
struct YYMMDD;

/*!
 *  @brief  GoogleCalendarAPI
 */
class GoogleCalendarAPI
{
public:
    /*!
     *  @param  config  GoogleCalendarAPI設定
     */
    GoogleCalendarAPI(const GoogleCalendarAPIConfigRef& config);
    /*!
     *  @param  api_key
     *  @param  calendar_id
     */
    GoogleCalendarAPI(const std::wstring& api_key,
                      const std::wstring& calendar_id);
    ~GoogleCalendarAPI();


    typedef std::function<void (bool b_result, const std::vector<YYMMDD>&)> GetHolidayCallback;
    typedef std::function<void (bool b_result, bool is_holiday)> IsHolidayCallback;

    /*!
     *  @brief  指定期間[start, end]の休日を得る
     *  @param  start       開始年月日
     *  @param  end         終了年月日
     *  @param  callback
     */
    void GetHoliday(const YYMMDD& start, const YYMMDD end, const GetHolidayCallback& callback);
    /*!
     *  @brief  指定日が休日か調べる
     *  @param  date        調べる年月日
     *  @param  callback
     */
    void IsHoliday(const YYMMDD& date, const IsHolidayCallback& callback);

private:
    GoogleCalendarAPI();
    GoogleCalendarAPI(const GoogleCalendarAPI&);
    GoogleCalendarAPI(GoogleCalendarAPI&&);
    GoogleCalendarAPI& operator= (const GoogleCalendarAPI&);

    class PIMPL;
    std::unique_ptr<PIMPL> m_pImpl;
};

} // namespace garnet
