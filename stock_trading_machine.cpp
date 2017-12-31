/*!
 *  @file   stock_trading_machine.h
 *  @brief  ���������F��
 *  @date   2017/05/05
 */
#include "stock_trading_machine.h"

#include "securities_session_sbi.h"
#include "stock_ordering_manager.h"
#include "stock_portfolio.h"
#include "stock_trading_starter_sbi.h"
#include "stock_trading_tactics.h"
#include "trade_assistant_setting.h"
#include "trade_struct.h"

#include "cipher_aes.h"
#include "holiday_investigator.h"
#include "random_generator.h"
#include "update_message.h"
#include "utility_datetime.h"
#include "yymmdd.h"

#include <codecvt>
#include <mutex>
#include <unordered_map>

namespace trading
{

class StockTradingMachine::PIMPL
{
private:
    enum eSequence
    {
        SEQ_ERROR,              //!< �G���[��~
        SEQ_INITIALIZE,         //!< ������
        SEQ_READY,              //!< ����OK
        SEQ_CLOSED_CHECK,       //!< ���؋x�꒲��
        SEQ_TRADING,            //!< �g���[�h�又��
        SEQ_WAIT,               //!< �C�ӂ̃E�F�C�g����
    };

    std::mutex m_mtx;       //!< �r������q
    eSequence m_sequence;   //!< �V�[�P���X

    eSecuritiesType m_securities;                               //!< �،���Ў��
    std::shared_ptr<SecuritiesSession> m_pSession;              //!< �،���ЂƂ̃Z�b�V����
    std::unique_ptr<StockTradingStarter> m_pStarter;            //!< ������X�^�[�^�[
    std::unique_ptr<StockOrderingManager> m_pOrderingManager;   //!< �����Ǘ���
    std::unique_ptr<HolidayInvestigator> m_pHolidayInvestigator;//!< �x��������

    std::vector<MMDD> m_jpx_holiday;                //!< JPX�ŗL�x�Ɠ�(�y���j�łȂ��Ƃ��x�݂ɂȂ��)
    std::vector<StockTimeTableUnit> m_timetable;    //!< ������^�C���e�[�u��
    std::vector<StockTradingTactics> m_tactics;     //!< ������헪�f�[�^
    std::unordered_map<eStockInvestmentsType, std::vector<StockPortfolio>> m_portfolio;   //!< �Ď������f�[�^

    RandomGenerator m_rand_gen; //!< ����������
    CipherAES m_aes_uid;        //!< �Í�uid
    CipherAES m_aes_pwd;        //!< �Í�pwd
    CipherAES m_aes_pwd_sub;    //!< �Í�pwd_sub

    int64_t m_tickcount;                        //!< �O�񑀍쎞��tickCount
    std::tm m_last_sv_time;                     //!< �Ō�ɃT�[�o(�،���Ђ���ыx������)���瓾������
    int64_t m_last_sv_time_tick;                //!< ���𓾂�tickCount
    int64_t m_wait_count;                       //!< �E�F�C�g�J�E���g(ms�P��)
    eSequence m_after_wait_seq;                 //!< �E�F�C�g�J���̑J�ڐ�V�[�P���X
    StockTimeTableUnit::eMode m_prev_tt_mode;   //!< �O��Update����TT���[�h
    int64_t m_last_monitor_tick;                //!< �Ď����������Ō�ɗv������tickCount

private:
    PIMPL();
    PIMPL(const PIMPL&);
    PIMPL& operator= (const PIMPL&);

    /*!
     *  @brief  ����X�V�����F������
     *  @param[in]  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
     *  @param[out] o_message   ���b�Z�[�W(�i�[��)
     */
    void Update_Initialize(TradeAssistantSetting& script_mng, UpdateMessage& o_message)
    {
        m_sequence = SEQ_ERROR;
        //
        if (!script_mng.BuildJPXHoliday(o_message, m_jpx_holiday)) {
            return;
        }
        if (!script_mng.BuildStockTimeTable(o_message, m_timetable)) {
            return;
        }
        if (!script_mng.BuildStockTactics(o_message, m_tactics)) {
            return;
        }

        // �^�C���e�[�u���͎����~��
        std::sort(m_timetable.begin(), 
                  m_timetable.end(),
                  [](const StockTimeTableUnit& left, const StockTimeTableUnit& right)->bool
            {
                return right < left;
            });

        // �،���Ђ��Ƃ̏���������
        switch (m_securities)
        {
        case SEC_SBI:
            m_pSession.reset(new SecuritiesSessionSbi(script_mng));
            m_pStarter.reset(new StockTradingStarterSbi(m_pSession, script_mng));
            m_pOrderingManager.reset(new StockOrderingManager(m_pSession, script_mng));
            m_sequence = SEQ_READY;
            break;
        default:
            break;
        }
    }

    /*!
     *  @brief  ����X�V�����F���؋x�꒲��
     *  @param[out] o_message   ���b�Z�[�W(�i�[��)
     */
    void Update_ClosedCheck(UpdateMessage& o_message)
    {
        m_sequence = SEQ_WAIT;

        m_pHolidayInvestigator.reset(new HolidayInvestigator());
        m_pHolidayInvestigator->Investigate([this](bool b_result, bool is_holiday, const std::wstring& datetime) {
            // http�֘A�X���b�h����Ă΂��̂�lock���K�v
            std::lock_guard<std::mutex> lock(m_mtx);
            //
            const uint32_t ACCEPTABLE_DIFF_SECONDS = 10*60; // ���e����鎞�ԃY��(10��)
            /*std::string dummy("Fri, 03 Jan 2014 21:39:11 GMT");*/
            auto pt(utility::ToLocalTimeFromRFC1123(datetime));
            utility::ToTimeFromBoostPosixTime(pt, m_last_sv_time);
            m_last_sv_time_tick = utility::GetTickCountGeneral();
            if (ACCEPTABLE_DIFF_SECONDS >= utility::GetDiffSecondsFromLocalMachineTime(m_last_sv_time)) {
                
                //m_last_sv_time.tm_hour = 9;//
                //m_last_sv_time.tm_min = 19;//
                //m_last_sv_time.tm_sec = 25;//
                //m_last_sv_time.tm_wday = 1;
                is_holiday = false;

                // �y���Ȃ�T�����ɍĒ���(���ۂɊ֌W�Ȃ�)
                m_after_wait_seq = SEQ_CLOSED_CHECK;
                if (utility::SATURDAY == m_last_sv_time.tm_wday) {
                    const int32_t AFTER_DAY = 2;
                    m_wait_count = utility::GetAfterDayLimitMS(pt, AFTER_DAY);
                } else if (utility::SUNDAY == m_last_sv_time.tm_wday) {
                    const int32_t AFTER_DAY = 1;
                    m_wait_count = utility::GetAfterDayLimitMS(pt, AFTER_DAY);
                } else {
                    if (b_result) {
                        if (is_holiday || IsJPXHoliday(m_last_sv_time)) {
                            // �j���܂��͌ŗL�x�Ɠ� �� �����Ē���
                            const int32_t AFTER_DAY = 1;
                            m_wait_count = utility::GetAfterDayLimitMS(pt, AFTER_DAY);
                        } else {
                            // �c�Ɠ� �� �g���[�h�又����
                            m_sequence = SEQ_TRADING;
                        }
                    } else {
                        // �������s �� �w�莞�Ԍ�ɍă`�������W
                        const int32_t WAIT_MINUTES = 10;
                        m_wait_count = utility::ToMiliSecondsFromMinute(WAIT_MINUTES);
                    }
                }
            } else {
                // ���ԃY�����Ђǂ�������]�v�Ȃ��Ƃ͂������A�i���ɑ҂�����(�ً}���[�h)
            }
        });
    }

    /*!
     *  @brief  �^�C���e�[�u���X�V
     *  @param  tickCount   �o�ߎ���[�~���b]
     *  @param  now_tm      ���ݎ���(�t�@�W�[)
     */
    StockTimeTableUnit::eMode CorrectTimeTable(int64_t tickCount, const std::tm& now_tm)
    {
        StockTimeTableUnit::eMode prev_mode = m_prev_tt_mode;
        StockTimeTableUnit::eMode next_mode = StockTimeTableUnit::CLOSED;
        StockTimeTableUnit now_tt(now_tm.tm_hour, now_tm.tm_min, now_tm.tm_sec);
        {
            bool is_smallest = true;
            for (const auto& tt : m_timetable) {
                next_mode = now_tt.m_mode;
                now_tt.m_mode = tt.m_mode;
                if (tt < now_tt) {
                    is_smallest = false;
                    break;
                }
            }
            if (is_smallest) {
                now_tt.m_mode = StockTimeTableUnit::CLOSED;
            }
        }
        bool b_valid = true;
        if (prev_mode != now_tt.m_mode) {
            auto initPortfolio = [this](eStockInvestmentsType investments_type, const std::vector<std::pair<uint32_t, std::string>>& rcv_portfolio)->bool {
                return InitPortfolio(investments_type, rcv_portfolio);
            };
            std::vector<uint32_t> monitoring_code;
            if (StockTimeTableUnit::IDLE == now_tt.m_mode) {
                // IDLE�ɕω�������A���̎��̃��[�h�̎�����ŊJ�n�������s
                eStockInvestmentsType start_inv = StockTimeTableUnit::ToInvestmentsTypeFromMode(next_mode);
                GetMonitoringCode(monitoring_code);
                b_valid = m_pStarter->Start(tickCount, m_aes_uid, m_aes_pwd, monitoring_code, start_inv, initPortfolio);
            } else if (StockTimeTableUnit::TOKYO == now_tt.m_mode || StockTimeTableUnit::PTS == now_tt.m_mode) {
                // �������[�h�ɕω�������J�n�������s
                eStockInvestmentsType start_inv = StockTimeTableUnit::ToInvestmentsTypeFromMode(now_tt.m_mode);
                GetMonitoringCode(monitoring_code);
                b_valid = m_pStarter->Start(tickCount, m_aes_uid, m_aes_pwd, monitoring_code, start_inv, initPortfolio);
            }
        }
        if (b_valid) {
            return now_tt.m_mode;
        } else {
            // �X�^�[�g�Ɏ��s������]�v�Ȃ��Ƃ͂������A�i���ɑ҂�����(�ً}���[�h)
            // ���O��̃X�^�[�g���I����ĂȂ��ꍇ�̂ݔ���
            // �������O���Ă�Ƃ��Ƃ��H
            m_sequence = SEQ_WAIT;
            return StockTimeTableUnit::CLOSED;
        }
    }
    /*!
     *  @brief  ����X�V�����F�g���[�h�又��
     *  @param[in]  tickCount   �o�ߎ���[�~���b]
     *  @param[in]  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
     *  @param[out] o_message   ���b�Z�[�W(�i�[��)
     */
    void Update_MainTrade(int64_t tickCount, TradeAssistantSetting& script_mng, UpdateMessage& o_message)
    {
        // �T�[�o�^�C���Ƀ��[�J���̌o�ߎ��Ԃ��������t�@�W�[�Ȍ��ݎ���
        std::tm now_tm;
        utility::AddTimeAndDiffMS(m_last_sv_time, (tickCount-m_last_sv_time_tick), now_tm);
        //
        auto now_mode = CorrectTimeTable(tickCount, now_tm);

        if (StockTimeTableUnit::TOKYO == now_mode || StockTimeTableUnit::PTS == now_mode) {
            const eStockInvestmentsType investments_type = StockTimeTableUnit::ToInvestmentsTypeFromMode(now_mode);
            if (m_pStarter->IsReady()) {
                // �Ď��������X�V
                const int64_t intv_ms = utility::ToMiliSecondsFromSecond(script_mng.GetStockValueIntervalSecond());
                if ((tickCount - m_last_monitor_tick) > intv_ms) {
                    m_last_monitor_tick = tickCount;
                    // �擾�v��
                    m_pSession->UpdateValueData([this, investments_type](bool b_success, const std::wstring& sendtime, const std::vector<RcvStockValueData>& rcv_valuedata) {
                        if (b_success) {
                            UpdateValueData(investments_type, sendtime, rcv_valuedata);
                        }
                    });
                }
                // �헪����
                const HHMMSS hhmmss(now_tm.tm_hour, now_tm.tm_min, now_tm.tm_sec);
                m_pOrderingManager->InterpretTactics(tickCount, hhmmss, investments_type, m_tactics, m_portfolio[investments_type], script_mng);
                // ����
                m_pOrderingManager->IssueOrder(m_aes_pwd_sub);
            }
        }

        m_prev_tt_mode = now_mode;
    }

    /*!
     *  @brief  ����X�V�����F�ėp�E�F�C�g����
     *  @param  tickCount   �o�ߎ���[�~���b]
     */
    void Update_Wait(int64_t tickCount)
    {
        if (m_wait_count > 0) {
            const int64_t past_tick = tickCount - m_tickcount;
            if (past_tick >= m_wait_count) {
                m_wait_count = 0;
                m_sequence = m_after_wait_seq;
            } else {
                m_wait_count -= past_tick;
            }
        }
    }

private:
    /*!
     *  @brief  JPX�̌ŗL�x�Ɠ���
     *  @param  src ���ׂ��
     *  @retval true    �x�Ɠ���
     */
    bool IsJPXHoliday(const std::tm& src) const
    {
        const MMDD srcmmdd(src.tm_mon+1, src.tm_mday); // tm��1����0
        for (const auto& mmdd: m_jpx_holiday) {
            if (mmdd == srcmmdd) {
                return true;
            }
        }
        return false;
    }

    /*!
     *  @brief  ���i�f�[�^�X�V
     *  @param  investments_type    ��������
     *  @param  senddate            ���i�f�[�^���M����
     *  @param  rcv_valuedata       �󂯎�������i�f�[�^
     */
    void UpdateValueData(eStockInvestmentsType investments_type, const std::wstring& sendtime, const std::vector<RcvStockValueData>& rcv_valuedata)
    {
        std::lock_guard<std::mutex> lock(m_mtx); // http�֘A�X���b�h����Ă΂��̂�lock���K�v

        auto portfolio_it = m_portfolio.find(investments_type);
        if (portfolio_it != m_portfolio.end()) {
            std::tm tm_send; // ���i�f�[�^���M����(�T�[�o�^�C��)
            auto pt(utility::ToLocalTimeFromRFC1123(sendtime));
            utility::ToTimeFromBoostPosixTime(pt, tm_send);
            std::vector<StockPortfolio>& portfolio(portfolio_it->second);
            for (const auto& vunit: rcv_valuedata) {
                uint32_t code = vunit.m_code;
                auto it = std::find_if(portfolio.begin(), portfolio.end(), [code](const StockPortfolio& pfu) {
                    return (pfu.m_code.GetCode() == code);
                });
                if (it != portfolio.end()) {
                    it->UpdateValueData(vunit, tm_send);
                } else {
                    // ������Ȃ�������ǂ�����H(error)
                }
            }
        } else {
            // �Ȃ�������������ĂȂ�(error)
        }
    }

    /*!
     *  @brief  �|�[�g�t�H���I������
     *  @param  investments_type    ��������
     *  @param  rcv_portfolio       ��M�����|�[�g�t�H���I(�Ȉ�)
     *  @retval true                ����
     */
    bool InitPortfolio(eStockInvestmentsType investments_type,  const std::vector<std::pair<uint32_t, std::string>>& rcv_portfolio)
    {
        std::lock_guard<std::mutex> lock(m_mtx); // http�֘A�X���b�h����Ă΂��̂�lock���K�v

        // ��M�����|�[�g�t�H���I���Ď������ƈ�v���Ă邩�`�F�b�N
        std::vector<StockPortfolio> portfolio;
        std::vector<uint32_t> monitoring_code;
        GetMonitoringCode(monitoring_code);
        portfolio.reserve(monitoring_code.size());
        for (uint32_t code: monitoring_code) {
            auto it = std::find_if(rcv_portfolio.begin(),
                                   rcv_portfolio.end(),
                                   [code](const std::pair<uint32_t, std::string>& rcv_pf_unit) {
                return rcv_pf_unit.first == code;
            });
            if (it != rcv_portfolio.end()) {
                portfolio.emplace_back(code, it->second);
            } else {
                return false;
            }
        }
        // �󂾂�����V�K�쐬
        auto portfolio_it = m_portfolio.find(investments_type);
        if (portfolio_it == m_portfolio.end()) {
            m_portfolio.emplace(investments_type, portfolio);
        }
        return true;
    }

    /*!
     *  @brief  �Ď������̃R�[�h�������o
     *  @param[out] dst �i�[��
     */
    void GetMonitoringCode(std::vector<uint32_t>& dst)
    {
        for (const StockTradingTactics& tactics: m_tactics) {
            std::vector<StockCode> code_vec;
            tactics.GetCode(code_vec);
            for (const StockCode& scode: code_vec) {
                if (std::find(dst.begin(), dst.end(), scode.GetCode()) == dst.end()) {
                    dst.emplace_back(scode.GetCode());
                }
            }
        }
    }

public:
    /*!
     *  @param  securities  �،���Ў��
     */
    PIMPL(eSecuritiesType securities)
    : m_sequence(SEQ_INITIALIZE)
    , m_securities(securities)
    , m_pSession()
    , m_pStarter()
    , m_pOrderingManager()
    , m_pHolidayInvestigator()
    , m_jpx_holiday()
    , m_timetable()
    , m_tactics()
    , m_portfolio()
    , m_rand_gen()
    , m_aes_uid()
    , m_aes_pwd()
    , m_aes_pwd_sub()
    , m_tickcount(0)
    , m_last_sv_time()
    , m_last_sv_time_tick(0)
    , m_wait_count(0)
    , m_after_wait_seq(SEQ_ERROR)
    , m_prev_tt_mode(StockTimeTableUnit::CLOSED)
    , m_last_monitor_tick(0)
    {
        memset(reinterpret_cast<void*>(&m_last_sv_time), 0, sizeof(m_last_sv_time));
    }

    /*!
     *  @brief  �g���[�h�J�n�ł��邩�H
     *  @retval true    �J�n�ł���
     */
    bool IsReady() const
    {
        return m_sequence == SEQ_READY;
    }

    /*!
     *  @brief  �g���[�h�J�n
     *  @param  uid
     *  @param  pwd
     *  @param  pwd_sub
     */
    void Start(const std::wstring& uid, const std::wstring& pwd, const std::wstring& pwd_sub)
    {
        // �Í������ĕێ�
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
        m_aes_uid.Encrypt(m_rand_gen, cv.to_bytes(uid));
        m_aes_pwd.Encrypt(m_rand_gen, cv.to_bytes(pwd));
        m_aes_pwd_sub.Encrypt(m_rand_gen, cv.to_bytes(pwd_sub));
        // �x��`�F�b�N��
        m_sequence = SEQ_CLOSED_CHECK;
    }

    /*!
     *  @brief  ����X�V����
     *  @param[in]  tickCount   �o�ߎ���[�~���b]
     *  @param[in]  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
     *  @param[out] o_message   ���b�Z�[�W(�i�[��)
     */
    void Update(int64_t tickCount, TradeAssistantSetting& script_mng,  UpdateMessage& o_message)
    {
        std::lock_guard<std::mutex> lock(m_mtx); // Update���͎�M���荞�݋֎~

        switch(m_sequence)
        {
        case SEQ_INITIALIZE:
            Update_Initialize(script_mng, o_message);
            break;
        case SEQ_CLOSED_CHECK:
            Update_ClosedCheck(o_message);
            break;
        case SEQ_TRADING:
            Update_MainTrade(tickCount, script_mng, o_message);
            break;
        case SEQ_WAIT:
            Update_Wait(tickCount);
        default:
            break;
        }

        m_tickcount = tickCount;
    }
};

/*!
 *  @param  securities  �،���Ў��
 */
StockTradingMachine::StockTradingMachine(eSecuritiesType securities)
: TradingMachine()
, m_pImpl(new PIMPL(securities))
{
}
/*!
 */
StockTradingMachine::~StockTradingMachine()
{
}

/*!
 *  @brief  �g���[�h�J�n�ł��邩�H
 */
bool StockTradingMachine::IsReady() const
{
    return m_pImpl->IsReady();
}

/*!
 *  @brief  �g���[�h�J�n
 *  @param  tickCount   �o�ߎ���[�~���b]
 *  @param  uid
 *  @param  pwd
 *  @param  pwd_sub
 */
void StockTradingMachine::Start(int64_t tickCount, const std::wstring& uid, const std::wstring& pwd, const std::wstring& pwd_sub)
{
    m_pImpl->Start(uid, pwd, pwd_sub);
}

/*!
 *  @brief  Update�֐�
 *  @param[in]  tickCount   �o�ߎ���[�~���b]
 *  @param[in]  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
 *  @param[out] o_message   ���b�Z�[�W(�i�[��)
 */
void StockTradingMachine::Update(int64_t tickCount, TradeAssistantSetting& script_mng, UpdateMessage& o_message)
{
    m_pImpl->Update(tickCount, script_mng, o_message);
}

} // namespace trading
