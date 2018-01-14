/*!
 *  @file   utility_debug.h
 *  @brief  [common]debug�֘AUtility�֐�
 *  @date   2017/12/31
 */
#pragma once

#include <string>

namespace garnet
{
namespace utility_debug
{

/*!
 *  @brief  [Debug]utf-16�������debug�o�͂���(�C�~�f�B�G�C�g)
 *  @param  str_u16
 */
void DebugOutput(const std::wstring& str_u16);
/*!
 *  @brief  [Debug]utf-8�������debug�o�͂���(�C�~�f�B�G�C�g)
 *  @param  str_u8
 */
void DebugOutput(const std::string& str_u8);

} // namespace utility_debug
} // namespace garnet
