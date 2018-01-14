/*!
 *  @file   twitter_config.h
 *  @brief  [common]twitter�ݒ�
 *  @date   2018/01/14
 *  @note   boost �Ɉˑ����Ă�
 */
#pragma once

#include <string>

namespace garnet
{
/*!
 *  @brief  twitter�ݒ�
 *  @note   ini�t�@�C����ǂݍ���ŊO���ɒ�
 */
class twitter_config
{
public:
    /*!
     *  @brief  �󐶐�
     */
    twitter_config();
    /*!
     *  @param  ini�t�@�C����
     */
    twitter_config(const std::string& ini_file_name);

    /*!
     *  @brief  ConsumerKey�𓾂�
     */
    const std::string& GetConsumerKey() const { return m_consumer_key; }
    /*!
     *  @brief  ConsumerSecret�𓾂�
     */
    const std::string& GetConsumerSecret() const { return m_consumer_secret; }
    /*!
     *  @brief  AccessToken�𓾂�
     */
    const std::string& GetAccessToken() const { return m_access_token; }
    /*!
     *  @brief  AccessTokenSecret�𓾂�
     */
    const std::string& GetAccessTokenSecret() const { return m_access_token_secret; }

    /*!
     *  @brief  �ő�c�C�[�g�������𓾂�
     */
    int32_t GetMaxTweetLetters() const { return m_max_tweet_letters; }

private:
    twitter_config(const twitter_config&);
    twitter_config(twitter_config&&);
    twitter_config& operator= (const twitter_config&);

    /*!
     *  @brief  ������
     *  @param  ini�t�@�C����
     */
    void initialize(const std::string& ini_file_name);

    std::string m_consumer_key;
    std::string m_consumer_secret;
    std::string m_access_token;
    std::string m_access_token_secret;

    //! �ő�c�C�[�g������
    int32_t m_max_tweet_letters;
};

} // namespace garnet
