/*!
 *  @file   trade_container.h
 *  @brief  �g���[�f�B���O�֘A�R���e�i
 *  @date   2018/01/06
 */
#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace trading
{

/*!
 *  @brief  ���������R�[�h�R���e�i<�����R�[�h>
 */
typedef std::unordered_set<uint32_t> StockCodeContainer;
/*!
 *  @brief  ���������R���e�i<�����R�[�h, ������(utf-16)>
 */
typedef std::unordered_map<uint32_t, std::wstring> StockBrandContainer;

/*!
 *  @brief  �������R���e�i<�����R�[�h, ����>
 */
typedef std::unordered_map<uint32_t, int32_t> SpotTradingsStockContainer;
/*!
 *  @brief  ���M�p���ʃR���e�i
 */
struct StockPosition;
typedef std::vector<StockPosition> StockPositionContainer;


} // namespace trading
