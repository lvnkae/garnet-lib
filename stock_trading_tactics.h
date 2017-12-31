/*!
 *  @file   stock_trading_tactics.h
 *  @brief  ������헪
 *  @date   2017/12/08
 */
#pragma once

#include <functional>
#include <vector>

struct HHMMSS;

namespace trading
{

class StockCode;
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

        int32_t Dbg_GetType() const { return static_cast<int32_t>(m_type); }
        int32_t Dbg_GetSignedParam() const { return m_signed_param; }
        float32 Dbg_GetFloatParam() const { return m_float_param; }

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
     *  @brief  �����ݒ�
     */
    class Order
    {
    private:
        int32_t m_group_id;     //!< �����O���[�v�ԍ�(����O���[�v�̒����͔r�����䂳���)
        eOrderType m_type;      //!< �^�C�v
        int32_t m_value_func;   //!< ���i�擾�֐�(���t�@�����X)
        int32_t m_volume;       //!< ����
        bool m_b_leverage;      //!< �M�p�t���O
        bool m_b_for_emergency; //!< �ً}���[�h�ł����s���閽�߂�
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
        : m_group_id(0)
        , m_type(eOrderType::ORDER_NONE)
        , m_volume(0)
        , m_value_func(0)
        , m_b_leverage(false)
        , m_b_for_emergency(false)
        , m_condition()
        {
        }

        void SetGroupID(int32_t group_id) { m_group_id = group_id; }

        void SetBuy(bool b_leverage, int32_t func_ref, int32_t volume) { SetParam(BUY, b_leverage, func_ref, volume); }
        void SetSell(bool b_leverage, int32_t func_ref, int32_t volume) { SetParam(SELL, b_leverage, func_ref, volume); }
        void SetCondition(const Trigger& trigger) { m_condition = trigger; }

        /*!
         *  @brief  �ً}���[�h�ł����s���閽�߂��H
         */
        bool IsForEmergency() const { return m_b_for_emergency; }
        /*!
         *  @brief  OrderType
         */
        eOrderType GetType() const { return m_type; }
        int32_t GetVolume() const { return m_volume; }
        bool GetIsLeverage() const { return m_b_leverage; }
        int32_t GetGroupID() const { return m_group_id; }
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

        const Trigger& Dbg_RefTrigger() const { return m_condition; }
    };

    /*!
     */
    StockTradingTactics();

    /*!
     *  @brief  �ŗLID�𓾂�
     */
    int32_t GetUniqueID() const { return m_unique_id; }
    /*!
     *  @brief  �����R�[�h�Q�𓾂�
     *  @param[out] dst �i�[��
     */
    void GetCode(std::vector<StockCode>& dst) const;

    /*!
     *  @brief  �ŗLID���Z�b�g����
     *  @note   unique�ł��邩�̓Z�b�g���鑤���ۏ؂��邱��
     */
    void SetUniqueID(int32_t id) { m_unique_id = id; }
    /*!
     *  @brief  �����R�[�h���Z�b�g����
     *  @param  code    �����R�[�h
     */
    void SetCode(uint32_t code);
    /*!
     *  @brief  �ً}���[�h�g���K�[��ǉ�����
     *  @param  trigger �g���K�[
     */
    void AddEmergencyTrigger(const Trigger& trigger);
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
    typedef std::function<void(int32_t, const StockTradingCommand&)> EnqueueFunc;

    /*!
     *  @brief  �헪����
     *  @param  b_emergency     �ً}���[�h��
     *  @param  hhmmss          ���ݎ����b
     *  @param  valuedata       ���i�f�[�^(1�������)
     *  @param  script_mng      �O���ݒ�(�X�N���v�g)�Ǘ���
     *  @param  enqueue_func    ���߂��L���[�ɓ����֐�
     */
    void Interpret(bool b_emergency,
                   const HHMMSS& hhmmss,
                   const std::vector<StockPortfolio>& valuedata,
                   TradeAssistantSetting& script_mng,
                   const EnqueueFunc& enqueue_func) const;

private:
    /*!
     *  @brief  �헪����(1������)
     *  @param  b_emergency     �ً}���[�h��
     *  @param  hhmmss          ���ݎ����b
     *  @param  valuedata       ���i�f�[�^(1������)
     *  @param  script_mng      �O���ݒ�(�X�N���v�g)�Ǘ���
     *  @param  enqueue_func    ���߂��L���[�ɓ����֐�
     */
    void InterpretAtCode(bool b_emergency,
                         const HHMMSS& hhmmss,
                         const StockPortfolio& valuedata,
                         TradeAssistantSetting& script_mng,
                         const EnqueueFunc& enqueue_func) const;


    int32_t m_unique_id;                //!< �ŗLID(�헪�f�[�^�ԂŔ��Ȃ�����)
    std::vector<StockCode> m_code;      //!< �����R�[�h(��̐헪�𕡐��̖����Ŏg�����Ƃ�����)
    std::vector<Trigger> m_emergency;   //!< �ً}���[�h�g���K�[
    std::vector<Order> m_fresh;         //!< �V�K�������X�g
    std::vector<Order> m_repayment;     //!< �ԍϒ������X�g
};

} // trading
