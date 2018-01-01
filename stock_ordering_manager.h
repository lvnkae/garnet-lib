/*!
 *  @file   stock_ordering_manager.h
 *  @brief  �������Ǘ���
 *  @date   2017/12/26
 */
#pragma once

#include "trade_define.h"

#include <memory>
#include <vector>

class CipherAES;
struct HHMMSS;
class TwitterSessionForAuthor;

namespace trading
{
class SecuritiesSession;
class StockTradingTactics;
struct StockPortfolio;
class TradeAssistantSetting;

/*!
 *  @brief  �������Ǘ���
 */
class StockOrderingManager
{
public:
    /*!
     *  @param  sec_session �،���ЂƂ̃Z�b�V����
     *  @param  tw_session  twitter�Ƃ̃Z�b�V����
     *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
     */
    StockOrderingManager(const std::shared_ptr<SecuritiesSession>& sec_session,
                         const std::shared_ptr<TwitterSessionForAuthor>& tw_session,
                         const TradeAssistantSetting& script_mng);
    /*!
     */
    ~StockOrderingManager();

    /*!
     *  @brief  ����헪����
     *  @param  tickCount   �o�ߎ���[�~���b]
     *  @param  hhmmss      ���ݎ����b
     *  @param  investments ��������
     *  @param  tactics     �헪�f�[�^
     *  @param  valuedata   ���i�f�[�^(1�������)
     *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
     *  @note   �헪�Ǝ����Ɖ��i�f�[�^���疽�߂𓾂�
     *  @note   �L���[�ɐςނ����Ŕ����܂ł͂��Ȃ�
     */
    void InterpretTactics(int64_t tickCount,
                          const HHMMSS& hhmmss,
                          eStockInvestmentsType investments,
                          const std::vector<StockTradingTactics>& tactics,
                          const std::vector<StockPortfolio>& valuedata,
                          TradeAssistantSetting& script_mng);

    /*!
     *  @brief  ���߂���������
     *  @param  aes_pwd
     */
    void IssueOrder(const CipherAES& aes_pwd);

private:
    StockOrderingManager();
    StockOrderingManager(const StockOrderingManager&);
    StockOrderingManager& operator= (const StockOrderingManager&);

    class PIMPL;
    std::unique_ptr<PIMPL> m_pImpl;
};

} // namespace trading
