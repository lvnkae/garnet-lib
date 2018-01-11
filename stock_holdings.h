/*!
 *  @file   stock_holdings.h
 *  @brief  ���ۗL����
 *  @date   2018/01/09
 */
#pragma once

#include "stock_code.h"

#include "yymmdd.h"

namespace trading
{

/*!
 *  @brief  �M�p���ۗL����(����)
 */
struct StockPosition
{
    StockCode m_code;       //!< �����R�[�h
    garnet::YYMMDD m_date;  //!< ����
    float64 m_value;        //!< ���P��
    int32_t m_number;       //!< ����
    bool m_b_sell;          //!< �����t���O

    StockPosition()
    : m_code()
    , m_date()
    , m_value(0.0)
    , m_number(0)
    , m_b_sell(false)
    {
    }
    StockPosition(uint32_t code,
                  const garnet::YYMMDD& date,
                  float64 value,
                  int32_t number,
                  bool b_sell)
    : m_code(code)
    , m_date(date)
    , m_value(value)
    , m_number(number)
    , m_b_sell(b_sell)
    {
    }

    /*!
     *  @note   ��r
     *  @param  right
     *  @retval true    ��v
     */
    bool operator==(const StockPosition& right) const;
};

} // namespace trading
