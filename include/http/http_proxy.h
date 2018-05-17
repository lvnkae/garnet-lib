/*!
 *  @file   http_proxy_magazine.h
 *  @brief  [common]proxy
 *  @date   2018/05/13
 *  @note   cpprestsdk �Ɉˑ�(namespace����������)
 */
#pragma once

#include "http/http_proxy_config_fwd.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace web
{
namespace http
{

namespace client { class http_client_config; }

/*!
 *  @brief  proxy�X�e�[�^�X
 */
struct proxy_status
{
    //! address:port
    std::wstring m_address_port;
    //! �ŏI�g�p����(tick)
    int64_t m_last_use_tick;
    //! �G���[�J�E���^
    int32_t m_error_cnt;

    proxy_status()
    : m_address_port()
    , m_last_use_tick(0)
    , m_error_cnt(0)
    {}
    proxy_status(std::wstring&& address_port)
    : m_address_port(address_port)
    , m_last_use_tick(0)
    , m_error_cnt(0)
    {}
};

/*!
 *  @brief  cookie_magazine�N���X
 *  @note   ������proxy����ꂽ�e�q
 */
class proxy_magazine
{
private:
    //! proxy�e�q
    std::vector< proxy_status> m_magazine;
    //! ���ݎg�p����proxy
    std::wstring m_current;
    //! �x������
    int64_t m_sleep_tick;
    //! �A���G���[���
    int32_t m_max_error_count;

public:
    /*!
     */
    proxy_magazine();
    /*!
     *  @param  config  proxy�ݒ�
     */
    proxy_magazine(const proxy_config_ref& config);
    /*!
     */
    ~proxy_magazine(){}

    /*!
     *  @brief  �ǉ��o�^
     *  @param  proxy   proxy������(address:port)
     */
    void entry(const std::string& proxy);

    /*!
     *  @brief  ����X�V
     *  @param  tick    �o�ߎ���[�~���b]
     *  @note   �󋵂ɉ�����proxy�̐؂�ւ���
     */
    void update(int64_t tick);

    /*!
     *  @brief  ���ݎg�p����proxy�𓾂�
     */
    std::wstring get_current() const { return m_current; }
    /*!
     *  @brief  ���ݎg�p����proxy���O��
     */
    void release_current() { m_current.swap(std::wstring()); }

    /*!
     *  @brief  �ŏI�g�p�����X�V
     *  @param  proxy   proxy������(address:port)
     *  @param  tick    �ŏI�g�p����(tick)[�~���b]
     */
    void update_tick(const std::wstring& proxy, int64_t tick);
    /*!
     *  @brief  �G���[�J�E���^�𑝂₷
     *  @param  proxy   proxy������(address:port)
     */
    void inc_error_count(const std::wstring& proxy);
    /*!
     *  @brief  �G���[�J�E���^���Z�b�g
     *  @param  proxy   proxy������(address:port)
     */
    void reset_error_count(const std::wstring& proxy);

private:
    proxy_magazine(const proxy_config&);
    proxy_magazine(proxy_config&&);
    proxy_magazine& operator= (const proxy_config&);
};

} // namespace http
} // namespace cookie
