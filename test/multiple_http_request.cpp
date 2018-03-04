#include "multiple_http_request.h"

#include "utility/utility_http.h"

#include "cpprest/http_client.h"
#include "cpprest/filestream.h"
#include <codecvt>
#include <iostream>
#include <functional>

namespace
{

typedef const std::function<void(web::http::http_response)> http_requst_callback;

enum J2_TEAM
{
    SHONAN,
    NAGASAKI,
    NAGOYA,
    FUKUOKA,
    TOKYO,
    CHIBA,
    TOKUSHIMA,
    MATSUMOTO,
    OITA,
    YOKOHAMA,
    YAMAGATA,
    KYOTO,
    OKAYAMA,
    MITO,
    EHIME,
    MACHIDA,
    KANAZAWA,
    GIFU,
    SANUKI,
    YAMAGUCHI,
    KUMAMOTO,
    KUSATSU,

    NUM_J2_TEAM
};

/*!
 *  @brief  SPORTSNAVI��J���[�O�`�[���g�b�v�y�[�W���擾����(���N�G�X�g���o��)
 *  @param  team_id     �`�[��ID
 *  @param  callback    �R�[���o�b�N
 */
void get_sportsnavi_jteam_top(int32_t team_id, const http_requst_callback& callback)
{
    const wchar_t BASE_URL[] = L"https://soccer.yahoo.co.jp/jleague/teams/detail/";
    
    const std::wstring url(std::move(std::wstring(BASE_URL) + std::to_wstring(team_id)));
    web::http::http_request request(web::http::methods::GET);
    garnet::utility_http::SetHttpCommonHeaderSimple(request);
    web::http::client::http_client http_client(url);
    http_client.request(request).then(callback);
}

}

/*!
 *  @brief  http response���猋�ʂ𓾂ăR���e�i�ɒǉ�����
 *  @param  rank        ����
 *  @param  response    http response
 */
void multiple_http_request::add_result_to_vector(int32_t rank, web::http::http_response& response)
{
    concurrency::streams::container_buffer<std::string> inStringBuffer;
    response.body().read_to_delim(inStringBuffer, 0).then([this, inStringBuffer, rank](size_t bytesRead)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mtx);

        const std::string html_u8(inStringBuffer.collection());
        const size_t work_max = 256;
        std::string work_str;
        if (html_u8.size() <= work_max) {
            work_str = html_u8;
        } else {
            work_str.reserve(work_max);
            work_str = std::move(html_u8.substr(0, work_max));
        }
        const size_t work_size = work_str.size();
        auto pos_first_hyphen = work_str.find('-');
        if (pos_first_hyphen == std::string::npos) {
            m_result.emplace(rank, L"");
            return;
        }
        auto pos_last_hyphen = work_str.find('-', pos_first_hyphen+1);
        if (pos_last_hyphen == std::string::npos) {
            m_result.emplace(rank, L"");
            return;
        }
        std::string team_name;
        const size_t team_name_top = pos_first_hyphen + 2;
        const size_t team_name_last = pos_last_hyphen - 2;
        const size_t team_name_len = team_name_last - team_name_top + 1;
        team_name.reserve(team_name_len);
        team_name = std::move(work_str.substr(team_name_top, team_name_len));
        team_name += '\0';
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utfconv;
        std::wstring work(std::move(utfconv.from_bytes(team_name)));
        m_result.emplace(rank, work);
    });
}

void multiple_http_request::muliple_request_test1()
{
    m_result.swap(std::map<int32_t, std::wstring>());

    const int32_t SPNAVI_TEAM_ID[NUM_J2_TEAM] = {
        130,    //!<  1�ʁF�Ó�
        30531,  //!<  2�ʁF����
        127,    //!<  3�ʁF���É�
        135,    //!<  4�ʁF����
        123,    //!<  5�ʁF����
        121,    //!<  6�ʁF��t
        30116,  //!<  7�ʁF����
        30528,  //!<  8�ʁF���{
        207,    //!<  9�ʁF�啪
        296,    //!< 10�ʁF���l
        294,    //!< 11�ʁF�R�`
        134,    //!< 12�ʁF���s
        30314,  //!< 13�ʁF���R
        176,    //!< 14�ʁF����
        30148,  //!< 15�ʁF���Q
        30532,  //!< 16�ʁF���c
        30526,  //!< 17�ʁF����
        30302,  //!< 18�ʁF��
        30536,  //!< 19�ʁF�]��
        30851,  //!< 20�ʁF�R��
        30303,  //!< 21�ʁF�F�{
        30103,  //!< 22�ʁF����
    };

    {
        const J2_TEAM team = SHONAN;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = NAGASAKI;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = NAGOYA;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = FUKUOKA;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = TOKYO;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = CHIBA;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = TOKUSHIMA;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = MATSUMOTO;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = OITA;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = YOKOHAMA;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = YAMAGATA;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = KYOTO;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = OKAYAMA;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = MITO;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = EHIME;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = MACHIDA;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = KANAZAWA;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = GIFU;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = SANUKI;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = YAMAGUCHI;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = KUMAMOTO;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
    {
        const J2_TEAM team = KUSATSU;
        const int32_t team_inx = static_cast<int32_t>(team);
        const int32_t rank = team_inx+1;
        get_sportsnavi_jteam_top(SPNAVI_TEAM_ID[team_inx],
                                 [this, rank](web::http::http_response response)
        {
            add_result_to_vector(rank, response);
        });
    }
}

bool multiple_http_request::is_end_of_multiple_request_test1() const
{
    return m_result.size() == static_cast<size_t>(NUM_J2_TEAM);
}

void multiple_http_request::print_result_of_multiple_request_test1() const
{
#if defined(WIN32)
    setlocale( LC_ALL, "Japanese" );
#endif/* WIN32 */

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utfconv;
    for (const auto& j2_team: m_result) {
        std::wcout << "(" << j2_team.first << ")" << j2_team.second.c_str() << std::endl;
    }
}


multiple_http_request::multiple_http_request()
: m_mtx()
, m_result()
{
}

multiple_http_request::~multiple_http_request()
{
}
