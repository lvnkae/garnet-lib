/*!
 *  @file   securities_session_sbi.h
 *  @brief  SBI�،��T�C�g�Ƃ̃Z�b�V�����Ǘ�
 *  @date   2017/05/05
 */
#pragma once

#include "securities_session.h"

#include <memory>

namespace trading
{

struct StockOrder;
class TradeAssistantSetting;

class SecuritiesSessionSbi : public SecuritiesSession
{
public:
    /*!
     *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
     */
    SecuritiesSessionSbi(const TradeAssistantSetting& script_mng);
    /*!
     */
    ~SecuritiesSessionSbi();

    /*!
     *  @breif  ���O�C��
     *  @param  uid
     *  @param  pwd
     *  @param  callback    �R�[���o�b�N
     *  @note   mobile��PC�̏��Ƀ��O�C��
     */
    void Login(const std::wstring& uid, const std::wstring& pwd, const LoginCallback& callback) override;
    /*!
     *  @brief  �|�[�g�t�H���I�쐬
     *  @param  monitoring_code     �Ď�����
     *  @param  investments_type    ����������
     *  @param  callback            �R�[���o�b�N
     *  @note   mobile�T�C�g�ŊĎ�������o�^����
     */
    void CreatePortfolio(const std::unordered_set<uint32_t>& monitoring_code,
                         eStockInvestmentsType investments_type,
                         const CreatePortfolioCallback& callback) override;
    /*!
     *  @brief  �|�[�g�t�H���I�]��
     *  @param  callback    �R�[���o�b�N
     *  @note   mobile�T�C�g����PC�T�C�g�ւ̓]��
     *  @note   mobile�T�C�g�͕\�����ڂ����Ȃ�����̂�PC�T�C�g�ֈڂ��Ă����炩����𓾂���
     */
    void TransmitPortfolio(const TransmitPortfolioCallback& callback) override;
    /*!
     *  @brief  ���i�f�[�^�X�V
     *  @param  callback    �R�[���o�b�N
     */
    void UpdateValueData(const UpdateValueDataCallback& callback) override;
    /*!
     *  @brief  �V�K��������
     *  @param  order       �������
     *  @param  pwd
     *  @param  callback    �R�[���o�b�N
     */
    void FreshOrder(const StockOrder& order, const std::wstring& pwd, const OrderCallback& callback) override;
    /*!
     *  @brief  ��������
     *  @param  order_id    �����ԍ�(�Ǘ��p)
     *  @param  order       �������
     *  @param  pwd
     *  @param  callback    �R�[���o�b�N
     */
    void CorrectOrder(int32_t order_id, const StockOrder& order, const std::wstring& pwd, const OrderCallback& callback) override;
    /*!
     *  @brief  �������
     *  @param  order_id    �����ԍ�(�Ǘ��p)
     *  @param  pwd
     *  @param  callback    �R�[���o�b�N
     */
    void CancelOrder(int32_t order_id, const std::wstring& pwd, const OrderCallback& callback) override;

    /*!
     *  @brief  �ۗL�������X�V
     */
    void UpdateStockOwned() override {}
    /*!
     *  @brief  �ۗL�����p����
     */
    void CloseLong(const StockOrder& order) override {}
    /*!
     *  @brief  �����߂�����
     */
    void CloseShort(const StockOrder& order) override {}

    /*!
     *  @brief  �،���ЃT�C�g�ŏI�A�N�Z�X�����擾
     *  @return �A�N�Z�X����(tickCount)
     *  @note   mobile��PC�ł��Â�����Ԃ�
     */
    int64_t GetLastAccessTime() const override;

private:
    class PIMPL;
    std::unique_ptr<PIMPL> m_pImpl;
};

} // namespace trading
