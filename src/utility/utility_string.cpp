/*!
 *  @file   utility_string.cpp
 *  @brief  [common]文字列操作Utility
 *  @date   2017/12/18
 */
#include "utility/utility_string.h"

#include "random_generator.h"

#include "boost/algorithm/string.hpp"
#include <vector>

namespace garnet
{
namespace utility_string
{

/*!
 *  @brief  ランダムなASCII文字列を得る
 *  @param[in]  rnd_gen 乱数生成器
 *  @param[in]  len     文字数
 *  @param[out] o_str   格納先
 */
void GetRandomString(RandomGenerator& rnd_gen, size_t len, std::string& o_string)
{
    if (len == 0) {
        return; // 不正
    }

    const int32_t USE_ASCII_NUMBERS = 10;
    const int32_t USE_ASCII_LETTERS = 26;
    const int32_t USE_ASCII_ALLS = USE_ASCII_NUMBERS + USE_ASCII_LETTERS * 2;

    std::vector<uint8_t> ascii_array;
    ascii_array.reserve(USE_ASCII_ALLS);
    for (int32_t inx = 0; inx < USE_ASCII_NUMBERS; inx++) {
        ascii_array.push_back('0' + inx);
    }
    for (int32_t inx = 0; inx < USE_ASCII_LETTERS; inx++) {
        ascii_array.push_back('A' + inx);
    }
    for (int32_t inx = 0; inx < USE_ASCII_LETTERS; inx++) {
        ascii_array.push_back('a' + inx);
    }

    o_string.reserve(len);
    for (size_t inx = 0; inx < len; inx++) {
        o_string.push_back(ascii_array[rnd_gen.Random(0, USE_ASCII_ALLS - 1)]);
    }
}

/*!
 *  @brief  string→wstringコピー
 *  @param  src 入力文字列
 *  @note   文字コードは一切考慮しないただのコピー
 */
std::wstring ToWstring(const std::string& src)
{
    std::wstring dst;
    dst.reserve(src.size());
    for (char c: src) {
        dst.push_back(c);
    }
    return dst;
}

/*!
 *  @brief  大文字→小文字変換
 *  @param[in]  src
 *  @param[out] dst
 */
template<typename SrcString, typename SrcChar>
void ToLowerCore(const SrcString& src, std::string& dst)
{
    const char DIFF_LOWER_AND_UPPER = ('a' - 'A');

    dst.reserve(src.size());
    for (const SrcChar c: src) {
        if (static_cast<SrcChar>('A') <= c && c <= static_cast<SrcChar>('Z')) {
            dst.push_back(static_cast<char>(c)+DIFF_LOWER_AND_UPPER);
        } else if (static_cast<SrcChar>(' ') <= c && c <= static_cast<SrcChar>('~')) {
            dst.push_back(static_cast<char>(c));
        }
    }
}
void ToLower(const std::wstring& src, std::string& dst) { ToLowerCore<std::wstring, wchar_t>(src, dst); }
void ToLower(const std::string& src, std::string& dst) { ToLowerCore<std::string, char>(src, dst); }

/*!
 *  @brief  文字列分割
 *  @param[in]  src
 *  @param[in]  delim   分割文字(デリミタ)
 *  @param[out] dst     結果格納先
 */
void Split(const std::string& src, const std::string& delim, std::vector<std::string>& dst)
{
    boost::algorithm::split(dst, src, boost::is_any_of(delim));
}
void Split(const std::string& src, const std::string& delim, std::unordered_set<std::string>& dst)
{
    boost::algorithm::split(dst, src, boost::is_any_of(delim));
}

/*!
 *  @brief  倍精度浮動小数を小数点以下N位まで文字列に変換
 *  @param  src     入力小数値
 *  @param  order   残す小数点桁数
 */
std::wstring ToWstringOrder(float64 src, uint32_t order)
{
    return ToWstring(std::move(ToStringOrder(src, order)));
}
/*!
 *  @brief  倍精度浮動小数を小数点以下N位まで文字列に変換
 *  @param  src     入力小数値
 *  @param  order   残す小数点桁数
 */
std::string ToStringOrder(float64 src, uint32_t order)
{
    std::string dst(std::to_string(src));
    auto pos = dst.find('.');
    if (pos != std::string::npos) {
        std::string::iterator it = dst.begin();
        if (order == 0) {
            it = it + pos;  // 0位指定なら'.'も消す
        } else {
            std::string::size_type del_point = pos + 1 + order;
            if (dst.size() <= del_point) {
                return dst; // 消す必要なし
            }
            it = it + del_point;
        }
        dst.erase(it, dst.end());
    }
    return dst;
}

/*!
 *  @brief  秘匿したいID(数値)を下N桁だけ文字列に変換
 *  @param  src     ID
 *  @param  order   表示する下桁数
 *  @return 文字列
 *  @note   上位桁は*で埋める
 */
std::wstring ToSecretIDOrder(int32_t src, uint32_t order)
{
    std::wstring dst(std::to_wstring(src));
    const size_t len = dst.size();
    if (len > order) {
        const size_t hide_len = len-order;
        for (size_t inx = 0; inx < hide_len; inx++) {
            dst[inx] = L'*';
        }
    }
    return dst;
}


} // namespace utility_string
} // namespace garnet
