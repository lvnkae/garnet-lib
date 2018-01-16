/*!
 *  @brief  http_client::requestを多重コールするテスト
 */
#pragma once

#include <mutex>
#include <map>
#include <string>

namespace web { namespace http { class http_response; } }

class multiple_http_request
{
public:
    multiple_http_request();
    ~multiple_http_request();

    /*!
     *  @brief  muliple_request_test1
     *  @note   sports naviのJ2全チームのトップページ取得リクエストを同時に出す
     *  @note   結果を2017年シーズンの順位と紐付けて格納
     */
    void muliple_request_test1();
    bool is_end_of_multiple_request_test1() const;
    void print_result_of_multiple_request_test1() const;

private:
    void add_result_to_vector(int32_t rank, web::http::http_response& response);

    std::recursive_mutex m_mtx;
    std::map<int32_t, std::wstring> m_result;
};
