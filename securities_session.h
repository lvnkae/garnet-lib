/*!
 *  @file   securities_session.h
 *  @brief  �،���ЃT�C�g�Ƃ̃Z�b�V�����Ǘ�
 *  @date   2017/05/05
 */
#pragma once

#include "trade_define.h"
#include <functional>
#include <string>
#include <vector>

namespace trading
{

struct StockOrder;
struct RcvStockValueData;
struct RcvResponseStockOrder;

class SecuritiesSession
{
public:
    typedef std::function<void (bool b_result, bool b_login, bool b_important_msg, const std::wstring&)> LoginCallback;
    typedef std::function<void (bool b_result, const std::vector<std::pair<uint32_t, std::string>>&)> CreatePortfolioCallback;
    typedef std::function<void (bool b_result)> TransmitPortfolioCallback;
    typedef std::function<void (bool b_result, const std::wstring&, const std::vector<RcvStockValueData>&)> UpdateValueDataCallback;
    typedef std::function<void (bool b_result, const RcvResponseStockOrder&, const std::wstring&)> OrderCallback;

    /*!
     *  @brief
     */
    SecuritiesSession();
    /*!
     *  @brief
     */
    virtual ~SecuritiesSession();


    /*!
     *  @breif  ���O�C��
     *  @param  uid
     *  @param  pwd
     *  @param  callback    �R�[���o�b�N
     */
    virtual void Login(const std::wstring& uid, const std::wstring& pwd, const LoginCallback& callback) = 0;
    /*!
     *  @brief  �|�[�g�t�H���I�쐬
     *  @param  monitoring_code     �Ď�����
     *  @param  investments_type    ����������
     *  @param  callback            �R�[���o�b�N
     */
    virtual void CreatePortfolio(const std::vector<uint32_t>& monitoring_code,
                                 eStockInvestmentsType investments_type,
                                 const CreatePortfolioCallback& callback) = 0;
    /*!
     *  @brief  �|�[�g�t�H���I�]��
     *  @param  callback    �R�[���o�b�N
     *  @note   SBI�ł����g��Ȃ��͂�
     */
    virtual void TransmitPortfolio(const TransmitPortfolioCallback& callback) = 0;
    /*!
     *  @brief  ���i�f�[�^�X�V
     *  @param  callback    �R�[���o�b�N
     */
    virtual void UpdateValueData(const UpdateValueDataCallback& callback) = 0;
    /*!
     *  @brief  �V�K��������
     *  @param  order       �������
     *  @param  pwd
     *  @param  callback    �R�[���o�b�N
     */
    virtual void FreshOrder(const StockOrder& order, const std::wstring& pwd, const OrderCallback& callback) = 0;

    /*!
     *  @brief  �ۗL�������X�V
     */
    virtual void UpdateStockOwned() = 0;
    /*!
     *  @brief  �ۗL�����p����
     */
    virtual void CloseLong(const StockOrder& order) = 0;
    /*!
     *  @brief  �����߂�����
     */
    virtual void CloseShort(const StockOrder& order) = 0;

    /*!
     *  @brief  �،���ЃT�C�g�ŏI�A�N�Z�X�����擾
     *  @return �A�N�Z�X����
     */
    virtual int64_t GetLastAccessTime() const = 0;

private:
    SecuritiesSession(const SecuritiesSession&);
    SecuritiesSession& operator= (const SecuritiesSession&);
};

} // namespace trading
