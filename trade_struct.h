/*!
 *  @file   trade_struct.h
 *  @brief  �g���[�f�B���O�֘A�\����
 *  @date   2017/12/21
 */
#pragma once

#include "hhmmss.h"
#include "trade_define.h"
#include "stock_code.h"
#include <string>

namespace trading
{

struct RcvResponseStockOrder;

/*!
 *  @brief  ������^�C���e�[�u��(��R�}��)
 */
struct StockTimeTableUnit
{
    enum eMode
    {
        CLOSED, //!< ��(�������Ȃ�)
        IDLE,   //!< ��MODE��������
        TOKYO,  //!< ���Z�،�������Ŕ���
        PTS,    //!< ���ݎ�����Ŕ���
    };

    HHMMSS m_hhmmss;//!< �����b
    eMode m_mode;   //!< ���[�h

    StockTimeTableUnit()
    : m_hhmmss()
    , m_mode(CLOSED)
    {
    }
    StockTimeTableUnit(int32_t h, int32_t m, int32_t s)
    : m_hhmmss(h, m, s)
    , m_mode(CLOSED)
    {
    }

    /*!
     *  @brief  ���[�h������(�X�N���v�g�p)���������ʂɕϊ�
     *  @param  mode_str    �^�C���e�[�u�����[�h������
     */
    static eStockInvestmentsType ToInvestmentsTypeFromMode(eMode tt_mode)
    {
        if (tt_mode == StockTimeTableUnit::TOKYO) {
            return INVESTMENTS_TOKYO;
        } else if (tt_mode == StockTimeTableUnit::PTS) {
            return INVESTMENTS_PTS;
        } else {
            // ����/����/�D�؂͑Ή����Ȃ�
            return INVESTMENTS_NONE;
        }
    }

    /*!
     *  @brief  ���[�h�ݒ�
     *  @param  mode_str    �^�C���e�[�u�����[�h������
     *  @retval true    ����
     */
    bool SetMode(const std::string& mode_str);

    /*!
     *  @brief  ��r����
     *  @param  right   �E�Ӓl
     *  @retval true    �E�Ӓl���傫��
     */
    bool operator<(const StockTimeTableUnit& right) const {
        return m_hhmmss < right.m_hhmmss;
    }
};

/*!
 *  @brief  �������p�����[�^
 */
 struct StockOrder
{
    StockCode m_code;       //!< �����R�[�h
    uint32_t m_number;      //!< ����
    float64 m_value;        //!< ���i
    bool m_b_leverage;      //!< �M�p�t���O
    bool m_b_market_order;  //!< ���s�t���O

    eOrderType m_type;                      //!< �������
    eOrderConditon m_condition;             //!< ����
    eStockInvestmentsType m_investiments;   //!< �����

    StockOrder()
    : m_code()
    , m_number(0)
    , m_value(0.f)
    , m_b_leverage(false)
    , m_b_market_order(false)
    , m_type(ORDER_NONE)
    , m_condition(CONDITION_NONE)
    , m_investiments(INVESTMENTS_NONE)
    {
    }

    /*!
     *  @param  rcv �����p�����[�^(��M�`��)
     */
    StockOrder(const RcvResponseStockOrder& rcv);

    /*!
     *  @brief  �����̐���`�F�b�N
     *  @retval true    ����
     */
    bool IsValid() const
    {
        if (!m_code.IsValid()) {
            return false;   // �s�������R�[�h
        }
        if (m_number == 0) {
            return false;   // �����w��Ȃ�
        }
        if (m_b_market_order) {
            if (m_value > 0.0) {
                // ���s + ���i��(�X�N���v�g�ł͉��i�}�C�i�X=���s�w��)
                return false;
            }
            if (m_condition == CONDITION_UNPROMOTED) {
                // ���s�� + �s��
                return false;
            }
        } else {
            if (static_cast<int32_t>(m_value) < 1) {
                // �񐬍s�� + ���i0�~��ƕ����͂��肦�Ȃ�
                return false;
            }
        }
        if (m_b_leverage) {
            if (m_investiments != INVESTMENTS_TOKYO) {
                // �M�p�͓��؈ȊO���肦�Ȃ�
                return false;
            }
        } else {
            if (m_investiments != INVESTMENTS_TOKYO && m_investiments != INVESTMENTS_PTS) {
                // ������s��(���w��܂��͖��Ή�)
                return false;
            }
        }
        if (m_type == ORDER_NONE) {
            // ������ʕs��
            return false;
        }
        //
        return true;
    }
};

/*!
 *  @brief  ����������[��M�`��]
 */
 struct RcvResponseStockOrder
{
    int32_t m_order_id;                 //!< �����ԍ�
    eOrderType m_type;                  //!< �������
    eStockInvestmentsType m_investments;//!< ��������
    uint32_t m_code;                    //!< �����R�[�h
    uint32_t m_number;                  //!< ��������
    float64 m_value;                    //!< �������i
    bool m_b_leverage;                  //!< �M�p�t���O

    RcvResponseStockOrder()
    : m_order_id(0)
    , m_type(ORDER_NONE)
    , m_investments(INVESTMENTS_NONE)
    , m_code(0)
    , m_number(0)
    , m_value(0.0)
    {
    }
};

} // namespace trading
