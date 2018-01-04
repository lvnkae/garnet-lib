/*!
 *  @file   stock_trading_command.h
 *  @brief  ���������
 *  @date   2017/12/27
 */
#pragma once

#include <string>
#include <unordered_set>
#include <vector>

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

    enum eIparam_Order
    {
        IPARAM_GROUP_ID,        //!< �헪�O���[�vID
        IPARAM_UNIQUE_ID,       //!< �헪�����ŗLID
        IPARAM_ORDER_TYPE,      //!< �������(eOrderType)
        IPARAM_ORDER_CONDITION, //!< ��������(eOrderConditon)
        IPARAM_B_LEVERAGE,      //!< �M�p����t���O
        IPARAM_NUMBER,          //!< ��������
        IPARAM_ORDER_ID,        //!< �����ԍ�(�،���Ђ����s��������/�Ǘ��p)

        NUM_IPARAM_ORDER,       //!< signed int�p�����[�^��
    };

    /*!
     *  @brief  �R�}���h����(�ً}���[�h)
     *  @param  code            �����R�[�h
     *  @param  tactics_id      �헪ID
     *  @param  target_group    �ΏۃO���[�v<�헪�O���[�vID>
     */
    StockTradingCommand(const StockCode& code,
                        int32_t tactics_id,
                        const std::unordered_set<int32_t>& target_group);
    /*!
     *  @brief  �R�}���h�쐬(����[����])
     *  @param  code        �����R�[�h
     *  @param  tactics_id  �헪ID
     *  @param  group_id    �헪�O���[�vID
     *  @param  unique_id   �헪�����ŗLID
     *  @param  order_type  �������(eOrderType)
     *  @param  order_cond  ��������(eOrderCondition)
     *  @param  b_leverage  �M�p����t���O
     *  @param  number      ��������
     *  @param  value       �������i
     */
    StockTradingCommand(const StockCode& code,
                        int32_t tactics_id,
                        int32_t group_id,
                        int32_t unique_id,
                        int32_t order_type,
                        int32_t order_condition,
                        bool b_leverage,
                        int32_t number,
                        float64 value);
    /*!
     *  @brief  �R�}���h�쐬(����[����])
     *  @param  src_command �㏑�����锄������
     *  @param  order_type  �������
     *  @param  order_id    �����ԍ�(�،���Ђ����s��������/�Ǘ��p)
     */
    StockTradingCommand(const StockTradingCommand& src_command,
                        int32_t order_type,
                        int32_t order_id);
    /*!
     *  @brief  �R�}���h�쐬(����[���])
     *  @param  src_order   ��������������
     *  @param  tactics_id  �헪ID
     *  @param  group_id    �헪�O���[�vID
     *  @param  unique_id   �헪�����ŗLID
     *  @param  order_type  �������(eOrderType)
     *  @param  order_id    �����ԍ�(�،���Ђ����s��������/�Ǘ��p)
     */
    StockTradingCommand(const StockOrder& src_order,
                        int32_t tactics_id,
                        int32_t group_id,
                        int32_t unique_id,
                        int32_t order_type,
                        int32_t order_id);


    /*!
     *  @brief  ���ߎ�ʂ𓾂�
     */
    eType GetType() const { return m_type; }
    /*!
     *  @brief  �����R�[�h�𓾂�
     */
    uint32_t GetCode() const { return m_code; }
    /*!
     *  @brief  �헪ID�𓾂�
     */
    int32_t GetTacticsID() const { return m_tactics_id; }

    /*!
     *  @brief  �ً}���[�h�ΏۃO���[�v���Q�Ƃ���
     */
    const std::vector<int32_t>& RefEmergencyTargetGroup() const { return iparam; }
    /*!
     *  @brief  �����t���[�p�����[�^���}�[�W����
     *  @param  src
     */
    void MergeIntFreeparam(const std::vector<int32_t>& src);

    /*!
     *  @brief  �������p�����[�^�𓾂�
     *  @param[out] dst �i�[��
     *  @note   �������ʂ͐G��Ȃ�(�ۗL���ĂȂ��̂�)
     */
    void GetOrder(StockOrder& dst) const;
    /*!
     *  @brief  �������t���[�p�����[�^�擾�F�헪�O���[�vID
     */
    int32_t GetOrderGroupID() const { return (m_type == ORDER && !iparam.empty()) ?iparam[IPARAM_GROUP_ID] :-1; }
    /*!
     *  @brief  �������t���[�p�����[�^�擾�F�헪�����ŗLID
     */
    int32_t GetOrderUniqueID() const { return (m_type == ORDER && !iparam.empty()) ?iparam[IPARAM_UNIQUE_ID] :-1; }
    /*!
     *  @brief  �������t���[�p�����[�^�擾�F�������(eOrderType)
     */
    int32_t GetOrderType() const { return (m_type == ORDER && !iparam.empty()) ?iparam[IPARAM_ORDER_TYPE] :0/*ORDER_NONE*/; }
    /*!
     *  @brief  �������t���[�p�����[�^�擾�F�����ԍ�
     */
    int32_t GetOrderID() const { return (m_type == ORDER && !iparam.empty()) ?iparam[IPARAM_ORDER_ID] :-1; }
    /*!
     *  @brief  right�͏�ʂ̔����������H
     *  @param  right   ��r���閽��
     */
    bool IsUpperBuySellOrder(const StockTradingCommand& right) const;


private:
    eType m_type;           //!< ���ߎ��
    uint32_t m_code;        //!< �����R�[�h
    int32_t m_tactics_id;   //!< �����헪ID

    //! �t���[�p�����[�^(�����t������)
    std::vector<int32_t> iparam;
    //! �t���[�p�����[�^(�{���x��������)
    float64 fparam;
};

} // namespace trading
