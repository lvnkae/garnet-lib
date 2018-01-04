/*!
 *  @file   stock_ordering_manager.h
 *  @brief  �������Ǘ���
 *  @date   2017/12/26
 */
#pragma once

#include "trade_define.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class CipherAES;
struct HHMMSS;
class TwitterSessionForAuthor;

namespace trading
{
struct RcvStockValueData;
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
                         TradeAssistantSetting& script_mng);
    /*!
     */
    ~StockOrderingManager();

    /*!
     *  @brief  �Ď������R�[�h�擾
     *  @param[out] dst �i�[��
     */
    void GetMonitoringCode(std::unordered_set<uint32_t>& dst);
    /*!
     *  @brief  �|�[�g�t�H���I������
     *  @param  investments_type    ��������
     *  @param  rcv_portfolio       ��M�����|�[�g�t�H���I<�����R�[�h�ԍ�, ������(utf-16)>
     *  @retval true                ����
     */
    bool InitPortfolio(eStockInvestmentsType investments_type,
                       const std::unordered_map<uint32_t, std::wstring>& rcv_portfolio);
    /*!
     *  @brief  ���i�f�[�^�X�V
     *  @param  investments_type    ��������
     *  @param  senddate            ���i�f�[�^���M����
     *  @param  rcv_valuedata       �󂯎�������i�f�[�^
     */
    void UpdateValueData(eStockInvestmentsType investments_type,
                         const std::wstring& sendtime,
                         const std::vector<RcvStockValueData>& rcv_valuedata);

    /*!
     *  @brief  ����X�V
     *  @param  tickCount   �o�ߎ���[�~���b]
     *  @param  hhmmss      ���ݎ����b
     *  @param  investments ��������
     *  @param  aes_pwd
     *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
     */
    void Update(int64_t tickCount,
                const HHMMSS& hhmmss,
                eStockInvestmentsType investments,
                const CipherAES& aes_pwd,
                TradeAssistantSetting& script_mng);

private:
    StockOrderingManager();
    StockOrderingManager(const StockOrderingManager&);
    StockOrderingManager& operator= (const StockOrderingManager&);

    class PIMPL;
    std::unique_ptr<PIMPL> m_pImpl;
};

} // namespace trading
