/*!
 *  @file   utility_string.cpp
 *  @brief  [common]�����񑀍�Utility
 *  @date   2017/12/18
 */
#include "utility/utility_string.h"

#include "random_generator.h"
#include <vector>

namespace garnet
{
namespace utility_string
{

/*!
 *  @brief  �����_����ASCII������𓾂�
 *  @param[in]  rnd_gen ����������
 *  @param[in]  len     ������
 *  @param[out] o_str   �i�[��
 */
void GetRandomString(RandomGenerator& rnd_gen, size_t len, std::string& o_string)
{
    if (len == 0) {
        return; // �s��
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
 *  @brief  string��wstring�R�s�[
 *  @param  src ���͕�����
 *  @note   �����R�[�h�͈�؍l�����Ȃ������̃R�s�[
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
 *  @brief  �啶�����������ϊ�
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
 *  @brief  �{���x���������������_�ȉ�N�ʂ܂ŕ�����ɕϊ�
 *  @param[in]  src
 *  @param[out] dst �i�[��
 */
std::wstring ToWstringOrder(float64 src, uint32_t order)
{
    std::wstring dst(std::to_wstring(src));
    auto pos = dst.find(L'.');
    if (pos != std::wstring::npos) {
        std::wstring::iterator it = dst.begin();
        if (order == 0) {
            it = it + pos;  // 0�ʎw��Ȃ�'.'������
        } else {
            std::wstring::size_type del_point = pos + 1 + order;
            if (dst.size() <= del_point) {
                return dst; // �����K�v�Ȃ�
            }
            it = it + del_point;
        }
        dst.erase(it, dst.end());
    }
    return dst;
}

/*!
 *  @brief  �铽������ID(���l)����N������������ɕϊ�
 *  @param  src     ID
 *  @param  order   �\�����鉺����
 *  @return ������
 *  @note   ��ʌ���*�Ŗ��߂�
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
