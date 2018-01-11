/*!
 *  @file   stock_trading_command_fwd.h
 *  @brief  ���������forward
 *  @date   2018/01/07
 */
#pragma once

#include <memory>

namespace trading
{

/*!
 *  @brief  ������ߋ��L�|�C���^
 */
class StockTradingCommand;
typedef std::shared_ptr<StockTradingCommand> StockTradingCommandPtr;

} // namespace trading
