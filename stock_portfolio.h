/*!
 *  @file   stock_portfolio.h
 *  @brief  ���Ď������f�[�^
 *  @date   2017/12/24
 */
#pragma once

#include "hhmmss.h"
#include "stock_code.h"
#include <string>
#include <vector>

namespace std { struct tm; }
namespace trading
{
struct RcvStockValueData;

/*!
 *  @brief  ���Ď������f�[�^
 *  @note   ���n�񉿊i�f�[�^�Q�����C���Ȃ̂Ń|�[�g�t�H���I�Ƃ͌���񂩂�
 */
struct StockPortfolio
{
    struct stockValue
    {
        HHMMSS m_hhmmss;    //!< �����b
        float64 m_value;    //!< ���i
        int64_t m_number;   //!< �o����

        stockValue()
        : m_hhmmss()
        , m_value(0.f)
        , m_number(0)
        {
        }

        stockValue(int32_t h, int32_t m, int32_t s)
        : m_hhmmss(h, m, s)
        , m_value(0.f)
        , m_number(0)
        {
        }
    };

    StockCode m_code;   //!< �����R�[�h
    float64 m_open;     //!< �n�l
    float64 m_high;     //!< ���l
    float64 m_low;      //!< ���l
    float64 m_close;    //!< �O�c�Ɠ��I�l
    std::vector<stockValue> m_value_data;   //!< ���n�񉿊i�f�[�^�Q

    StockPortfolio()
    : m_code()
    , m_open(0.f)
    , m_high(0.f)
    , m_low(0.f)
    , m_close(0.f)
    , m_value_data()
    {
    }
    StockPortfolio(uint32_t scode)
    : m_code(scode)
    , m_open(0.f)
    , m_high(0.f)
    , m_low(0.f)
    , m_close(0.f)
    , m_value_data()
    {
    }

    /*!
     *  @brief  ���i�f�[�^�X�V
     *  @param  src     ���i�f�[�^
     *  @param  date    �擾����(�T�[�o���Ԃ��g��)
     */
    void UpdateValueData(const RcvStockValueData& src, const std::tm& date);
};

/*!
 *  @brief  ���i�f�[�^(1������)��M�`��
 */
struct RcvStockValueData
{
    uint32_t m_code;    //!< �����R�[�h
    float64 m_value;    //!< ���l
    float64 m_open;     //!< �n�l
    float64 m_high;     //!< ���l
    float64 m_low;      //!< ���l
    float64 m_close;    //!< �O�c�Ɠ��I�l    
    int64_t m_number;   //!< �o����

    RcvStockValueData()
    : m_code(0)
    , m_value(0.f)
    , m_open(0.f)
    , m_high(0.f)
    , m_low(0.f)
    , m_close(0.f)
    , m_number(0)
    {
    }
};

} // namespace trading

