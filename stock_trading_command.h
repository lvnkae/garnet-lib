/*!
 *  @file   stock_trading_command.h
 *  @brief  ���������
 *  @date   2017/12/27
 */
#pragma once

#include <string>

namespace trading
{

class StockCode;
struct StockOrder;

/*!
 *  @brief  ���������
 *  @note   ����헪���甭���Ǘ��ւ̎w���Ɏg��
 */
class StockTradingCommand
{
public:
    enum eType
    {
        NONE = 0,

        EMERGENCY,  //!< �ً}���[�h(�ʏ풍���S����&�ʏ픭���ꎞ��~)
        ORDER,      //!< ����
    };

    /*!
     *  @param  type    ���ߎ��
     *  @param  code    �����R�[�h
     *  @param  name    ������
     *  @param  tid     �헪ID
     *  @param  gid     �헪���O���[�vID
     */
    StockTradingCommand(eType type, const StockCode& code, const std::wstring& name, int32_t tid, int32_t gid);
    /*!
     *  @brief  �������i�ݒ�
     */
    void SetOrderParam(int32_t order_type,
                       int32_t order_condition,
                       uint32_t number,
                       float64 value,
                       bool b_leverage)
    { 
        param1 = order_type;
        param2 = order_condition;
        param3 = number;
        param4 = (b_leverage) ?1 :0;
        fparam0 = value;
    }

    /*!
     *  @brief  ���ߎ�ʂ𓾂�
     */
    eType GetType() const { return m_type; }
    /*!
     *  @brief  �����R�[�h�𓾂�
     */
    uint32_t GetCode() const { return m_code; }
    /*!
     *  @brief  ������(utf-16)�𓾂�
     */
    const std::wstring& GetName() const { return m_name; }
    /*!
     *  @brief  �헪ID�𓾂�
     */
    int32_t GetTacticsID() const { return m_tactics_id; }
    /*!
     *  @brief  �O���[�vID�𓾂�
     */
    int32_t GetGroupID() const { return m_group_id; }

    /*!
     *  @brief  �ً}�����߂�
     *  @note   �ً}������(�}�����_��/�}�ϓ����ی����؂�etc)�Ȃ�true
     *  @note   RESET�ΏۊO�ɂ������c
     */
    bool IsForEmergencyCommand() const
    {
        switch (m_type)
        {
        case ORDER:
            return param4 != 0;
        case EMERGENCY:
            return false;
        default:
            return false;
        }
    }

    /*!
     *  @brief  �������p�����[�^�𓾂�
     *  @param[out] dst �i�[��
     *  @note   �������ʂ͐G��Ȃ�(�ۗL���ĂȂ��̂�)
     */
    void GetOrder(StockOrder& dst) const;

private:
    eType m_type;           //!< ���ߎ��
    uint32_t m_code;        //!< �����R�[�h
    std::wstring m_name;    //!< ������(���b�Z�[�W�p/utf-16)
    int32_t m_tactics_id;   //!< �����헪ID
    int32_t m_group_id;     //!< �헪���O���[�vID

    int32_t param0;     //!< �t���[�p�����[�^(int32) [                /              ]
    int32_t param1;     //!< �t���[�p�����[�^(int32) [                /eOrderType    ]
    int32_t param2;     //!< �t���[�p�����[�^(int32) [                /eOrderConditon]
    uint32_t param3;    //!< �t���[�p�����[�^(int32) [                /����          ]
    uint32_t param4;    //!< �t���[�p�����[�^(int32) [�ً}�����߃t���O/�M�p�t���O    ]

    float64 fparam0;    //!< �t���[�p�����[�^(float) [                /�������i      ]
};

} // namespace trading
