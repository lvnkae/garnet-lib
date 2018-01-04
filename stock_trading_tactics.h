/*!
 *  @file   stock_trading_tactics.h
 *  @brief  ������헪
 *  @date   2017/12/08
 */
#pragma once

#include <functional>
#include <vector>
#include <unordered_set>

struct HHMMSS;

namespace trading
{

struct StockPortfolio;
class StockTradingCommand;
class TradeAssistantSetting;

/*!
*  @brief  ������헪
*/
class StockTradingTactics
{
private:
    enum eTriggerType
    {
        TRRIGER_NONE,

        VALUE_GAP,      // �}�ϓ�(����������t�ȓ��Ɋ���r�ω�(r>0:�㏸/r<0:����)
        NO_CONTRACT,    // �����Ԋu(����t�ȏ��肪�Ȃ�����)
        SCRIPT_FUNCTION,// �X�N���v�g�֐�����
    };
    enum eOrderType
    {
        ORDER_NONE,

        BUY,    // ��������
        SELL,   // ���蒍��
    };

public:
    /*!
     *  @brief  �g���K�[�ݒ�
     *  @note   ���^�����N�����^�C�~���O
     */
    class Trigger
    {
    private:
        eTriggerType m_type;       //!< �^�C�v
        float32 m_float_param;     //!< �t���[�p�����[�^(32bit���������_)
        int32_t m_signed_param;    //!< �t���[�p�����[�^(32bit�����t��)
    public:
        Trigger()
        : m_type(eTriggerType::TRRIGER_NONE)
        , m_float_param(0.f)
        , m_signed_param(0)
        {
        }

        void Set_ValueGap(float32 persent, int32_t sec)
        {
            m_type = VALUE_GAP;
            m_float_param = persent;
            m_signed_param = sec;
        }
        void Set_NoContract(int32_t sec)
        {
            m_type = NO_CONTRACT;
            m_signed_param = sec;
        }
        void Set_ScriptFunction(int32_t func_ref)
        {
            m_type = SCRIPT_FUNCTION;
            m_signed_param =func_ref;
        }

        bool empty() const { return m_type == TRRIGER_NONE; }

        /*!
         *  @brief  ����
         *  @param  hhmmss      ���ݎ����b
         *  @param  valuedata   ���i�f�[�^(1������)
         *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
         *  @retval true        �g���K�[����
         */
        bool Judge(const HHMMSS& hhmmss, const StockPortfolio& valuedata, TradeAssistantSetting& script_mng) const;
    };

    /*!
     *  @brief  �ً}���[�h�ݒ�
     */
    class Emergency
    {
    private:
        std::unordered_set<int32_t> m_group;    //!< �ΏۃO���[�v�ԍ�
        Trigger m_condition;                    //!< ��������

    public:
        Emergency()
        : m_group()
        , m_condition()
        {
        }

        void AddTargetGroupID(int32_t group_id) { m_group.insert(group_id); }
        void SetCondition(const Trigger& trigger) { m_condition = trigger; }

        const std::unordered_set<int32_t>& RefTargetGroup() const { return m_group; }
        bool empty() const { return m_condition.empty(); }

        /*!
         *  @brief  ����
         *  @param  hhmmss      ���ݎ����b
         *  @param  valuedata   ���i�f�[�^(1������)
         *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
         *  @retval true        �g���K�[����
         */
        bool Judge(const HHMMSS& hhmmss, const StockPortfolio& valuedata, TradeAssistantSetting& script_mng) const
        {
            return m_condition.Judge(hhmmss, valuedata, script_mng);
        }
    };

    /*!
     *  @brief  �����ݒ�
     */
    class Order
    {
    private:
        int32_t m_unique_id;    //!< �����ŗLID
        int32_t m_group_id;     //!< �헪�O���[�vID(����O���[�v�̒����͔r�����䂳���)
        eOrderType m_type;      //!< �^�C�v
        int32_t m_value_func;   //!< ���i�擾�֐�(���t�@�����X)
        int32_t m_volume;       //!< ����
        bool m_b_leverage;      //!< �M�p�t���O
        Trigger m_condition;    //!< ��������

        void SetParam(eOrderType type, bool b_leverage, int32_t func_ref, int32_t volume)
        {
            m_type = type;
            m_value_func = func_ref;
            m_volume = volume;
            m_b_leverage = b_leverage;
        }

    public:
        Order()
        : m_unique_id(0)
        , m_group_id(0)
        , m_type(eOrderType::ORDER_NONE)
        , m_volume(0)
        , m_value_func(0)
        , m_b_leverage(false)
        , m_condition()
        {
        }

        void SetUniqueID(int32_t unique_id) { m_unique_id = unique_id; }
        void SetGroupID(int32_t group_id) { m_group_id = group_id; }

        void SetBuy(bool b_leverage, int32_t func_ref, int32_t volume) { SetParam(BUY, b_leverage, func_ref, volume); }
        void SetSell(bool b_leverage, int32_t func_ref, int32_t volume) { SetParam(SELL, b_leverage, func_ref, volume); }
        void SetCondition(const Trigger& trigger) { m_condition = trigger; }

        /*!
         *  @brief  OrderType�𓾂�
         */
        eOrderType GetType() const { return m_type; }
        int32_t GetGroupID() const { return m_group_id; }
        int32_t GetUniqueID() const { return m_unique_id; }
        int32_t GetVolume()  const { return m_volume; }
        bool GetIsLeverage() const { return m_b_leverage; }
        /*!
         *  @brief  ���i�擾�֐��Q�Ǝ擾
         */
        int32_t GetValueFuncReference() const { return m_value_func; }
        /*!
         *  @brief  ����
         *  @param  hhmmss      ���ݎ����b
         *  @param  valuedata   ���i�f�[�^(1������)
         *  @param  script_mng  �O���ݒ�(�X�N���v�g)�Ǘ���
         *  @retval true        �g���K�[����
         */
        bool Judge(const HHMMSS& hhmmss, const StockPortfolio& valuedata, TradeAssistantSetting& script_mng) const
        {
            return m_condition.Judge(hhmmss, valuedata, script_mng);
        }
    };

    /*!
     */
    StockTradingTactics();

    /*!
     *  @brief  �ŗLID�𓾂�
     */
    int32_t GetUniqueID() const { return m_unique_id; }

    /*!
     *  @brief  �ŗLID���Z�b�g����
     *  @note   unique�ł��邩�̓Z�b�g���鑤���ۏ؂��邱��
     */
    void SetUniqueID(int32_t id) { m_unique_id = id; }
    /*!
     *  @brief  �ً}���[�h��ǉ�����
     *  @param  emergency   �ً}���[�h�ݒ�
     */
    void AddEmergencyMode(const Emergency& emergency);
    /*!
     *  @brief  �V�K������ǉ�
     *  @param  order   �����ݒ�
     */
    void AddFreshOrder(const Order& order);
    /*!
     *  @brief  �ԍϒ�����ǉ�
     *  @param  order   �����ݒ�
     */
    void AddRepaymentOrder(const Order& order);

    /*!
     */
    typedef std::function<void(const StockTradingCommand&)> EnqueueFunc;

    /*!
     *  @brief  �헪����
     *  @param  hhmmss          ���ݎ����b
     *  @param  em_group        �ً}���[�h�ΏۃO���[�v<�헪�O���[�vID>
     *  @param  valuedata       ���i�f�[�^(1������)
     *  @param  script_mng      �O���ݒ�(�X�N���v�g)�Ǘ���
     *  @param  enqueue_func    ���߂��L���[�ɓ����֐�
     */
    void Interpret(const HHMMSS& hhmmss,
                   const std::unordered_set<int32_t>& em_group,
                   const StockPortfolio& valuedata,
                   TradeAssistantSetting& script_mng,
                   const EnqueueFunc& enqueue_func) const;

private:
    int32_t m_unique_id;                //!< �ŗLID(�헪�f�[�^�ԂŔ��Ȃ�����)
    std::vector<Emergency> m_emergency; //!< �ً}���[�h���X�g
    std::vector<Order> m_fresh;         //!< �V�K�������X�g
    std::vector<Order> m_repayment;     //!< �ԍϒ������X�g
};

} // trading
