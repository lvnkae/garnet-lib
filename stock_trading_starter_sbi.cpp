/*!
 *  @file   stock_trading_starter_sbi.cpp
 *  @brief  ������X�^�[�g�W�FSBI�p
 *  @date   2017/12/20
 */
#include "stock_trading_starter_sbi.h"

#include "securities_session_sbi.h"
#include "trade_assistant_setting.h"

#include "cipher_aes.h"
#include "random_generator.h"
#include "twitter_session.h"
#include "utility_datetime.h"

namespace trading
{

class StockTradingStarterSbi::PIMPL
{
private:
    enum eSequence
    {
        SEQ_NONE,   //!< �������ĂȂ�
        SEQ_BUSY,   //!< ������
        SEQ_READY,  //!< ����OK
    };

    const int64_t m_session_keep_ms;                        //!< �،���ЂƂ̃Z�b�V�����𖳃A�N�Z�X�ňێ��ł��鎞��(ms)
    std::shared_ptr<SecuritiesSession> m_pSecSession;       //!< �،���ЂƂ̃Z�b�V����
    std::shared_ptr<TwitterSessionForAuthor> m_pTwSession;  //!< twitter�Ƃ̃Z�b�V����(���b�Z�[�W�ʒm�p)

    eSequence m_sequence;                                   //!< �V�[�P���X
    eStockInvestmentsType m_last_portfolior_investments;    //!< �Ō�ɍ�����|�[�g�t�H���I�̑Ώێ�������

    /*!
     *  @brief  �|�[�g�t�H���I�쐬
     *  @param  monitoring_code     �Ď�����
     *  @param  investments_type    ��������
     *  @param  init_portfolio      �|�[�g�t�H���I�������֐�
     */
    void CreatePortfolio(const std::unordered_set<uint32_t>& monitoring_code,
                         eStockInvestmentsType investments_type,
                         const InitPortfolioFunc& init_portfolio)
    {
        if (m_last_portfolior_investments != investments_type) {
            m_pSecSession->CreatePortfolio(monitoring_code,
                                           investments_type,
                                           [this, 
                                            investments_type,
                                            init_portfolio](bool b_result,
                                                            const std::unordered_map<uint32_t, std::wstring>& rcv_portfolio)
            {
                bool b_valid = false;
                if (b_result) {
                    b_valid = init_portfolio(investments_type, rcv_portfolio);
                }
                if (b_valid) {
                    m_pSecSession->TransmitPortfolio([this](bool b_result)
                    {
                        if (b_result) {
                            m_sequence = SEQ_READY;
                        } else {
                            // ���s�����ꍇ��BUSY�̂܂�(�X�^�[�^�[�Ăяo�����őΏ�)
                        }
                    });
                } else {
                    // ���s�����ꍇ��BUSY�̂܂�(�X�^�[�^�[�Ăяo�����őΏ�)
                }
            });
            m_last_portfolior_investments = investments_type;
            m_sequence = SEQ_BUSY;
        } else {
            // �O��Ɠ���������Ȃ̂ō쐬�s�v
            m_sequence = SEQ_READY;
        }
    }

public:
    /*!
     *  @param  sec_session �،���ЂƂ̃Z�b�V����
     *  @param  tw_session  twitter�Ƃ̃Z�b�V����
     *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
     */
    PIMPL(const std::shared_ptr<SecuritiesSession>& sec_session,
          const std::shared_ptr<TwitterSessionForAuthor>& tw_session,
          const TradeAssistantSetting& script_mng)
    : m_session_keep_ms(utility::ToMiliSecondsFromMinute(script_mng.GetSessionKeepMinute()))
    , m_pSecSession(sec_session)
    , m_pTwSession(tw_session)
    , m_sequence(SEQ_NONE)
    , m_last_portfolior_investments(INVESTMENTS_NONE)
    {
    }

    /*!
     *  @brief  ����������ł��Ă邩
     *  @retval true    ����OK
     */
    bool IsReady() const
    {
        return m_sequence == SEQ_READY;
    }

    /*!
     *  @brief  �J�n����
     *  @param  tickCount           �o�ߎ���[�~���b]
     *  @param  aes_uid
     *  @param  aes_pwd
     *  @param  monitoring_code     �Ď�����
     *  @param  investments_type    ��������
     *  @param  init_portfolio      �|�[�g�t�H���I�������֐�
     *  @retval true                ����
     */
    bool Start(int64_t tickCount,
               const CipherAES& aes_uid,
               const CipherAES& aes_pwd,
               const std::unordered_set<uint32_t>& monitoring_code,
               eStockInvestmentsType investments_type,
               const InitPortfolioFunc& init_portfolio)
    {
        if (m_sequence != SEQ_NONE && m_sequence != SEQ_READY) {
            return false; // �J�n������
        }

        const int64_t diff_tick = tickCount - m_pSecSession->GetLastAccessTime();
        if (diff_tick > m_session_keep_ms) {
            // SBI�ւ̍ŏI�A�N�Z�X����̌o�ߎ��Ԃ��Z�b�V�����ێ����Ԃ𒴂��Ă����烍�O�C�����s
            std::wstring uid, pwd;
            aes_uid.Decrypt(uid);
            aes_pwd.Decrypt(pwd);
            m_pSecSession->Login(uid, pwd,
                                 [this, monitoring_code,
                                        investments_type,
                                        init_portfolio](bool b_result,
                                                        bool b_login,
                                                        bool b_important_msg,
                                                        const std::wstring& sv_date)
            {
                if (!b_result) {
                    m_pTwSession->Tweet(sv_date, L"���O�C���G���[�B�ً}�����e�i���X����������܂���B");
                } else if (!b_login) {
                    m_pTwSession->Tweet(sv_date, L"���O�C���ł��܂���ł����BID�܂��̓p�X���[�h���Ⴂ�܂��B");
                    // >ToDo< �ē��͂ł���悤�ɂ���(�g���[�h�J�n�{�^�������Amachine�V�[�P���X���Z�b�g)
                } else {
                    if (b_important_msg) {
                        m_pTwSession->Tweet(sv_date, L"���O�C�����܂����BSBI����̏d�v�Ȃ��m�点������܂��B");
                    } else {
                        m_pTwSession->Tweet(sv_date, L"���O�C�����܂���");
                    }
                    // �|�[�g�t�H���I�쐬
                    CreatePortfolio(monitoring_code, investments_type, init_portfolio);
                }
            });
            m_sequence = SEQ_BUSY;
        } else {
            CreatePortfolio(monitoring_code, investments_type, init_portfolio);
        }
        return true;
    }
};

/*!
 *  @param  sec_session �،���ЂƂ̃Z�b�V����
 *  @param  tw_session  twitter�Ƃ̃Z�b�V����
 *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
 */
StockTradingStarterSbi::StockTradingStarterSbi(const std::shared_ptr<SecuritiesSession>& sec_session,
                                               const std::shared_ptr<TwitterSessionForAuthor>& tw_session,
                                               const TradeAssistantSetting& script_mng)
: StockTradingStarter()
, m_pImpl(new PIMPL(sec_session, tw_session, script_mng))
{
}
/*!
 */
StockTradingStarterSbi::~StockTradingStarterSbi()
{
}

/*!
 *  @brief  ����������ł��Ă邩
 */
bool StockTradingStarterSbi::IsReady() const
{
    return m_pImpl->IsReady();
}

/*!
 *  @brief  �J�n����
 *  @param  tickCount           �o�ߎ���[�~���b]
 *  @param  aes_uid
 *  @param  aes_pwd
 *  @param  monitoring_code     �Ď�����
 *  @param  investments_type    ��������
 *  @param  init_portfolio      �|�[�g�t�H���I�������֐�
 */
bool StockTradingStarterSbi::Start(int64_t tickCount,
                                   const CipherAES& aes_uid,
                                   const CipherAES& aes_pwd,
                                   const std::unordered_set<uint32_t>& monitoring_code,
                                   eStockInvestmentsType investments_type,
                                   const InitPortfolioFunc& init_portfolio)
{
    return m_pImpl->Start(tickCount, aes_uid, aes_pwd, monitoring_code, investments_type, init_portfolio);
}

} // namespace trading
