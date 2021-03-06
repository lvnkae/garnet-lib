/*!
 *  @file   utility_string.h
 *  @brief  [common]文字列操作Utility
 *  @date   2017/12/18
 *  @note   RandomGenerator, boost::algorithm に依存している
 */
#pragma once

#include <string>
#include <unordered_set>
#include <vector>

namespace garnet
{
class RandomGenerator;

namespace utility_string
{

/*!
 *  @brief  ランダムなASCII文字列を得る
 *  @param[in]  rnd_gen 乱数生成器
 *  @param[in]  len     文字数
 *  @param[out] o_str   格納先
 *  @note   ASCIIの数字・英字(大小)の文字列を得る。記号は含まない。
 */
void GetRandomString(RandomGenerator& rnd_gen, size_t len, std::string& o_string);

/*!
 *  @brief  string→wstringコピー
 *  @param  src 入力文字列
 *  @note   文字コードは一切考慮しないただのコピー
 */
std::wstring ToWstring(const std::string& src);

/*!
 *  @brief  大文字→小文字変換
 *  @param[in]  src
 *  @param[out] dst
 *  @note   マルチバイト文字は切り捨てる
 *  @note   UTF-8は正しい結果が得られない(マルチバイト文字が混じってるとその一部がアルファベットに化ける)
 */
void ToLower(const std::wstring& src, std::string& dst);
void ToLower(const std::string& src, std::string& dst);

/*!
 *  @brief  文字列分割
 *  @param[in]  src
 *  @param[in]  delim   分割文字(デリミタ)
 *  @param[out] dst     結果格納先
 */
void Split(const std::string& src, const std::string& delim, std::vector<std::string>& dst);
void Split(const std::string& src, const std::string& delim, std::unordered_set<std::string>& dst);

/*!
 *  @brief  倍精度浮動小数を小数点第n位まで文字列に変換
 *  @param  src     入力小数値
 *  @param  order   残す小数点桁数
 *  @return 文字列
 *  @note   std::to_wstring/std::to_stringだと桁数指定ができないので
 */
std::wstring ToWstringOrder(float64 src, uint32_t order);
std::string ToStringOrder(float64 src, uint32_t order);
/*!
 *  @brief  秘匿したいID(数値)を下N桁だけ文字列に変換
 *  @param  src     ID
 *  @param  order   表示する下桁数
 *  @return 文字列
 *  @note   上位桁は*で埋める
 */
std::wstring ToSecretIDOrder(int32_t src, uint32_t order);

} // namespace utility_string
} // namespace garnet
