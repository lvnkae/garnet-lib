/*!
 *  @brief  http_client::request�𑽏d�R�[������e�X�g
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
     *  @note   sports navi��J2�S�`�[���̃g�b�v�y�[�W�擾���N�G�X�g�𓯎��ɏo��
     *  @note   ���ʂ�2017�N�V�[�Y���̏��ʂƕR�t���Ċi�[
     */
    void muliple_request_test1();
    bool is_end_of_multiple_request_test1() const;
    void print_result_of_multiple_request_test1() const;

private:
    void add_result_to_vector(int32_t rank, web::http::http_response& response);

    std::recursive_mutex m_mtx;
    std::map<int32_t, std::wstring> m_result;
};
