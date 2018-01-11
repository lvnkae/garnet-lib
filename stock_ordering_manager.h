/*!
 *  @file   stock_ordering_manager.h
 *  @brief  �������Ǘ���
 *  @date   2017/12/26
 */
#pragma once

#include "securities_session_fwd.h"
#include "trade_container.h"
#include "trade_define.h"

#include "twitter_session_fwd.h"

#include <memory>
#include <vector>

namespace garnet
{
class CipherAES;
struct HHMMSS;
} // namespace garnet

namespace trading
{
struct RcvStockValueData;
struct StockExecInfoAtOrder;
class StockTradingTactics;
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
    StockOrderingManager(const SecuritiesSessionPtr& sec_session,
                         const garnet::TwitterSessionForAuthorPtr& tw_session,
                         TradeAssistantSetting& script_mng);
    /*!
     */
    ~StockOrderingManager();

    /*!
     *  @brief  �Ď������R�[�h�擾
     *  @param[out] dst �i�[��
     */
    void GetMonitoringCode(StockCodeContainer& dst);
    /*!
     *  @brief  �Ď�����������
     *  @param  investments_type    ��������
     *  @param  rcv_brand_data      ��M�����Ď������Q
     *  @retval true                ����
     */
    bool InitMonitoringBrand(eStockInvestmentsType investments_type,
                             const StockBrandContainer& rcv_brand_data);

    /*!
     *  @brief  �ۗL�����X�V
     *  @param  spot        �����ۗL��
     *  @param  position    �M�p�ۗL��
     */
    void UpdateHoldings(const SpotTradingsStockContainer& spot,
                        const StockPositionContainer& position);

    /*!
     *  @brief  �Ď��������i�f�[�^�X�V
     *  @param  investments_type    ��������
     *  @param  senddate            ���i�f�[�^���M����
     *  @param  rcv_valuedata       �󂯎�������i�f�[�^
     */
    void UpdateValueData(eStockInvestmentsType investments_type,
                         const std::wstring& sendtime,
                         const std::vector<RcvStockValueData>& rcv_valuedata);
    /*!
     *  @brief  ���������X�V
     *  @param  rcv_info    �󂯎���������
     */
    void UpdateExecInfo(const std::vector<StockExecInfoAtOrder>& rcv_info);

    /*!
     *  @brief  Update�֐�
     *  @param  tickCount   �o�ߎ���[�~���b]
     *  @param  hhmmss      ���ݎ����b
     *  @param  investments ��������
     *  @param  aes_pwd
     *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
     */
    void Update(int64_t tickCount,
                const garnet::HHMMSS& hhmmss,
                eStockInvestmentsType investments,
                const garnet::CipherAES& aes_pwd,
                TradeAssistantSetting& script_mng);

private:
    StockOrderingManager();
    StockOrderingManager(const StockOrderingManager&);
    StockOrderingManager(const StockOrderingManager&&);
    StockOrderingManager& operator= (const StockOrderingManager&);

    class PIMPL;
    std::unique_ptr<PIMPL> m_pImpl;
};

} // namespace trading
