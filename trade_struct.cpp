/*!
 *  @file   trade_struct.cpp
 *  @brief  �g���[�f�B���O�֘A�\����
 *  @date   2017/12/21
 */
#include "trade_struct.h"

namespace trading
{
/*!
 *  @brief  ���������R�[�h�F�Ȃ�
 */
const uint32_t STOCK_CODE_NONE = 0;

/*!
 *  @brief
 */
StockCode::StockCode()
: m_code(STOCK_CODE_NONE)
{
}

/*!
*  @brief
*/
StockCode::StockCode(uint32_t code)
: m_code(code)
{
}

/*!
 *  @brief  ���[�h�ݒ�
 *  @param  mode_str    �^�C���e�[�u�����[�h������
 */
bool StockTimeTableUnit::SetMode(const std::string& mode_str)
{
    if (mode_str.compare("CLOSED") == 0) {
        m_mode = CLOSED;
    } else if (mode_str.compare("IDLE") == 0) {
        m_mode = IDLE;
    } else if (mode_str.compare("TOKYO") == 0) {
        m_mode = TOKYO;
    } else if (mode_str.compare("PTS") == 0) {
        m_mode = PTS;
    } else {
        return false;
    }
    return true;
}

} // namespace trading
