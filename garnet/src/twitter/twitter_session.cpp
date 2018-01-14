/*!
 *  @file   twitter_session.cpp
 *  @brief  [common]twitterAPI�Z�b�V����
 *  @date   2017/12/31
 */
#include "twitter_session.h"

#include "twitter_config.h"
#include "utility_datetime.h"
#include "utility_http.h"

#include "cpprest/http_client.h"
#include "cpprest/oauth1.h"
#include <codecvt>

namespace garnet
{
namespace twitter
{

/*!
 *  @brief  ���s�R�[�h������𓾂�
 */
const std::wstring GetNewlineString() { return L"\n"; }

}

class TwitterSessionForAuthor::PIMPL
{
private:
    const std::wstring m_consumer_key;
    const std::wstring m_consumer_secret;
    const std::wstring m_access_token;
    const std::wstring m_access_token_secret;

    const size_t m_max_tweet_letters;
    
    PIMPL(const PIMPL&);
    PIMPL& operator= (const PIMPL&);

public:
    /*!
     *  @param  consumer_key
     *  @param  consumer_secret
     *  @param  access_token
     *  @param  access_token_secret
     *  @param  max_tweet_letters   �ő�c�C�[�g������
     */
    PIMPL(const std::wstring& consumer_key,
          const std::wstring& consumer_secret,
          const std::wstring& access_token,
          const std::wstring& access_token_secret,
          size_t max_tweet_letters)
    : m_consumer_key(consumer_key)
    , m_consumer_secret(consumer_secret)
    , m_access_token(access_token)
    , m_access_token_secret(access_token_secret)
    , m_max_tweet_letters(max_tweet_letters)
    {
    }

    /*!
     *  @brief  �c�C�[�g����
     *  @param  src �c�C�[�g������(utf-16)
     *  @return true    ���M����
     */
    bool Tweet(const std::wstring& src)
    {
        if (src.size() > m_max_tweet_letters) {
            return false;   // ��������
        }

        web::http::http_request request(web::http::methods::POST);
        utility_http::SetHttpCommonHeaderSimple(request);
        web::http::oauth1::experimental::oauth1_config oa1_conf(m_consumer_key,
                                                                m_consumer_secret,
                                                                L"https://api.twitter.com/oauth/request_token",
                                                                L"https://api.twitter.com/oauth/authorize",
                                                                L"https://api.twitter.com/oauth/access_token",
                                                                L"http://127.0.0.1",
                                                                web::http::oauth1::experimental::oauth1_methods::hmac_sha1);
        
        oa1_conf.set_token(std::move(web::http::oauth1::experimental::oauth1_token(m_access_token, m_access_token_secret)));
        web::http::client::http_client_config conf;
        conf.set_oauth1(oa1_conf);
        //
        std::wstring enc_src(std::move(web::uri::encode_data_string(src))); // tweet�������URL�G���R�[�h���Ă���
        std::wstring form_data;
        utility_http::AddFormDataParamToString(L"status", enc_src, form_data);
        utility_http::SetFormData(form_data, request);
        //
        web::http::client::http_client http_client(L"https://api.twitter.com/1.1/statuses/update.json", conf);
        http_client.request(request);
        
        return true;
    }
};

/*!
 *  @param  config  twitter�ݒ�
 */
TwitterSessionForAuthor::TwitterSessionForAuthor(const twitter_config_ref& config)
: m_pImpl()
{
    const twitter_config_ptr p = config.lock();
    if (nullptr != p) {
        const twitter_config& tw_conf(*p);
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utfconv;
        m_pImpl.reset(new PIMPL(std::move(utfconv.from_bytes(tw_conf.GetConsumerKey())),
                                std::move(utfconv.from_bytes(tw_conf.GetConsumerSecret())),
                                std::move(utfconv.from_bytes(tw_conf.GetAccessToken())),
                                std::move(utfconv.from_bytes(tw_conf.GetAccessTokenSecret())),
                                tw_conf.GetMaxTweetLetters()));
    } else {
        m_pImpl.reset(new PIMPL(std::wstring(),
                                std::wstring(),
                                std::wstring(),
                                std::wstring(),
                                0));
    }
}
/*!
 *  @param  consumer_key
 *  @param  consumer_secret
 *  @param  access_token
 *  @param  access_token_secret
 *  @param  max_tweet_letters   �ő�c�C�[�g������
 */
TwitterSessionForAuthor::TwitterSessionForAuthor(const std::wstring& consumer_key,
                                                 const std::wstring& consumer_secret,
                                                 const std::wstring& access_token,
                                                 const std::wstring& access_token_secret,
                                                 size_t max_tweet_letters)
: m_pImpl(new PIMPL(consumer_key,
                    consumer_secret,
                    access_token,
                    access_token_secret,
                    max_tweet_letters))
{   
}
/*!
 */
TwitterSessionForAuthor::~TwitterSessionForAuthor()
{    
}

/*!
 *  @brief  �c�C�[�g����
 *  @param  date    ����������(ASCII)
 *  @param  src     �c�C�[�g������(utf-16)
 */
bool TwitterSessionForAuthor::Tweet(const std::wstring& date, const std::wstring& src)
{
    if (date.empty()) {
        // �����w�肳��ĂȂ�������V�X�e�����Ԃ�����
        using utility_datetime::GetLocalMachineTime;
        const std::wstring lc_date(std::move(GetLocalMachineTime(L"%a, %d %b %Y %H:%M:%S JST")));
        return m_pImpl->Tweet(lc_date + twitter::GetNewlineString() + src);
    } else {
        return m_pImpl->Tweet(date + twitter::GetNewlineString() + src);
    }
}
bool TwitterSessionForAuthor::Tweet(const std::wstring& src)
{
    return m_pImpl->Tweet(src);
}

} // namespace garnet