/*!
 *  @file   stock_trading_starter.h
 *  @brief  ������X�^�[�g�W
 *  @date   2017/12/20
 */
#pragma once

#include "trade_define.h"

#include <functional>
#include <vector>

class CipherAES;

namespace trading
{
class StockTradingStarter
{
public:
    typedef std::function<bool(eStockInvestmentsType, const std::vector<std::pair<uint32_t, std::string>>& rcv_portfolio)> InitPortfolioFunc;

    StockTradingStarter();
    ~StockTradingStarter();

    /*!
     *  @brief  ����������ł��Ă邩
     *  @retval true    ����OK
     */
    virtual bool IsReady() const = 0;

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
    virtual bool Start(int64_t tickCount,
                       const CipherAES& aes_uid,
                       const CipherAES& aes_pwd,
                       const std::vector<uint32_t>& monitoring_code,
                       eStockInvestmentsType investments_type,
                       const InitPortfolioFunc& init_portfolio) = 0;

private:
    StockTradingStarter(const StockTradingStarter&);
    StockTradingStarter& operator= (const StockTradingStarter&);
};

} // namespace trading
