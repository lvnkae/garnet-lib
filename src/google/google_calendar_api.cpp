/*!
 *  @file   google_calendar_api.cpp
 *  @brief  [common]GoogleCalendarAPI
 *  @date   2018/03/11
 */
#include "google/google_calendar_api.h"

#include "google/google_api_config.h"
#include "utility/utility_datetime.h"
#include "utility/utility_http.h"
#include "utility/utility_string.h"
#include "yymmdd.h"

#include "cpprest/http_client.h"
#include "cpprest/oauth1.h"
#include <codecvt>

namespace garnet
{

class GoogleCalendarAPI::PIMPL
{
private:
    const std::wstring m_api_key;
    const std::wstring m_calendar_id;

    PIMPL(const PIMPL&);
    PIMPL& operator= (const PIMPL&);

    /*!
     *  @brief  API�`���̓��t�𓾂�
     *  @param  date    �N����
     */
    static std::wstring GetAPIDate(const YYMMDD& date)
    {
        return utility_string::ToWstring(date.to_hyphen_string() + "T00:00:00.000Z");
    }

    /*!
     *  @brief  Calendar�A�N�Z�XURL�𓾂�
     *  @param  start   �J�n�N����
     *  @param  end     �I���N����
     */
    std::wstring CreateURL(const YYMMDD& start, const YYMMDD& end)
    {
        // "end"���܂ޔ͈͂ɂ������̂�max��1����
        const YYMMDD yymmdd_max = end.GetAfterDay(1);
        const int32_t diff_day = yymmdd_max.GetDiffDay(start);
        const std::wstring tm_min(std::move(GetAPIDate(start)));
        const std::wstring tm_max(std::move(GetAPIDate(yymmdd_max)));

        std::wstring url(L"https://www.googleapis.com/calendar/v3/calendars/"
                        + web::uri::encode_data_string(m_calendar_id)
                        + L"/events");
        utility_http::AddItemToURL(L"key", web::uri::encode_data_string(m_api_key), url);
        utility_http::AddItemToURL(L"timeMin", web::uri::encode_data_string(tm_min), url);
        utility_http::AddItemToURL(L"timeMax", web::uri::encode_data_string(tm_max), url);
        // �ő�x������max-min���Ƃ��Ă���
        utility_http::AddItemToURL(L"maxResults", std::to_wstring(diff_day), url);
        utility_http::AddItemToURL(L"orderBy", L"startTime", url);
        utility_http::AddItemToURL(L"singleEvents", L"true", url);

        return url;
    }

public:
    /*!
     *  @param  api_key
     *  @param  calendar_id
     */
    PIMPL(const std::wstring& api_key,
          const std::wstring& calendar_id)
    : m_api_key(api_key)
    , m_calendar_id(calendar_id)
    {
    }

    /*!
     *  @brief  �w������x�������ׂ�
     *  @param  date        ���ׂ�N����
     *  @param  callback
     */
    void IsHoliday(const YYMMDD& date, const IsHolidayCallback& callback)
    {
        const std::wstring url(CreateURL(date, date));
        web::http::http_request request(web::http::methods::GET);
        utility_http::SetHttpCommonHeaderSimple(request);
        //
        web::http::client::http_client http_client(url);
        http_client.request(request)
            .then([callback](web::http::http_response response) -> pplx::task<web::json::value>
        {
            if (response.status_code() == web::http::status_codes::OK) {
                return response.extract_json();
            } else {
                return pplx::task_from_result(web::json::value());
            }
        }).then([callback](pplx::task<web::json::value> previousTask) {
            const web::json::value& v = previousTask.get();
            if (v.is_null()) {
                callback(false, false);
            } else {
                const web::json::object vo = v.as_object();
                for (auto it = vo.cbegin(); it != vo.cend(); ++it) {
                    const std::wstring key_str = it->first;
                    if (key_str == L"items") {
                        // items����łȂ���΋x��
                        const web::json::array& item = it->second.as_array();
                        bool is_holiday = item.size() != 0;
                        callback(true, is_holiday);
                        return;
                    }
                }
                callback(false, false);
            }
        });
    }
};

/*!
 *  @param  config  GoogleCalendarAPI�ݒ�
 */
GoogleCalendarAPI::GoogleCalendarAPI(const GoogleCalendarAPIConfigRef& config)
: m_pImpl()
{
    const GoogleCalendarAPIConfigPtr p = config.lock();
    if (nullptr != p) {
        const GoogleCalendarAPIConfig& conf(*p);
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utfconv;
        m_pImpl.reset(new PIMPL(std::move(utfconv.from_bytes(conf.GetAPIKey())),
                                std::move(utfconv.from_bytes(conf.GetCarendarID()))));
    } else {
        m_pImpl.reset(new PIMPL(std::wstring(),
                                std::wstring()));
    }
}
/*!
 *  @param  api_key
 *  @param  calendar_id
 */
GoogleCalendarAPI::GoogleCalendarAPI(const std::wstring& api_key,
                                     const std::wstring& calendar_id)
: m_pImpl(new PIMPL(api_key, calendar_id))
{   
}
/*!
 */
GoogleCalendarAPI::~GoogleCalendarAPI()
{    
}

/*!
 *  @brief  �w������x�������ׂ�
 *  @param  date        ���ׂ�N����
 *  @param  callback
 */
void GoogleCalendarAPI::IsHoliday(const YYMMDD& date, const IsHolidayCallback& callback)
{
    m_pImpl->IsHoliday(date, callback);
}

} // namespace garnet
