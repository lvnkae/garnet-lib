/*!
 *  @file   lua_accessor.h
 *  @brief  [common]lua�Ƃ̂��Ƃ蒇��
 *  @date   2017/05/04
 *  @note   lua�Ɉˑ����Ă���
 */
#pragma once

#include <memory>
#include <stack>
#include <string>

struct lua_State;
class LuaAccessor;

/*!
 *  @brief  lua����N���X
 */
class LuaAccessor
{
public:
    LuaAccessor();
    ~LuaAccessor();

    /*!
     *  @brief  �t�@�C���ǂݍ���
     *  @param  file_name   �t�@�C�����t���p�X
     *  @retval true    ����
     *  @note   �������A
     */
    bool DoFile(const std::string& file_name);
    /*!
     *  @brief  lua�X�^�b�N��S�N���A����
     */
    void ClearStack();

    /*!
     *  @brief  �e�e�[�u�����J��
     *  @param  table_name  �e�[�u����
     *  @return �e�[�u���v�f��
     */
    int32_t OpenTable(const std::string& table_name);
    /*!
     *  @brief  �q�e�[�u�����J��
     *  @param  table_name  �q�e�[�u����
     *  @return �q�e�[�u���v�f��
     */
    int32_t OpenChildTable(const std::string& table_name);
    /*!
     *  @brief  �q�e�[�u�����J��
     *  @param  table_inx    �q�e�[�u���C���f�b�N�X(0�n�܂�)
     *  @return �q�e�[�u���v�f��
     */
    int32_t OpenChildTable(int32_t table_inx);
    /*!
     *  @brief  �Ō�ɊJ�����e�[�u�����N���[�Y����
     */
    void CloseTable();

    /*!
     *  @brief  �O���[�o���p�����[�^�擾
     *  @param[in]  param_name  �p�����[�^��
     *  @param[out] o_param     �p�����[�^�i�[��
     *  @retval     true        ����
     */
    bool GetGlobalParam(const std::string& param_name, std::string& o_param);
    bool GetGlobalParam(const std::string& param_name, int32_t& o_param);
    /*!
     *  @brief  �e�[�u���p�����[�^�擾
     *  @param[in]  param_name  �p�����[�^��
     *  @param[out] o_param     �p�����[�^�i�[��
     *  @retval     true        ����
     */
    bool GetTableParam(const std::string& param_name, std::string& o_param);
    bool GetTableParam(const std::string& param_name, int32_t& o_param);
    bool GetTableParam(const std::string& param_name, float32& o_param);
    bool GetTableParam(const std::string& param_name, bool& o_param);
    /*!
     *  @brief  �z��p�����[�^�擾
     *  @param[in]  param_inx   �p�����[�^�C���b�N�X(0�n�܂�)
     *  @param[out] o_param     �p�����[�^�i�[��
     *  @retval     true        ����
     */
    bool GetArrayParam(int32_t param_inx, std::string& o_param);
    bool GetArrayParam(int32_t param_inx, int32_t& o_param);

    /*!
     *  @brief  lua�֐����t�@�����X�擾
     *  @param[in]  param_name  �p�����[�^��(lua�֐��̊i�[���ꂽ�p�����[�^�̖��O)
     *  @param[out] o_ref       �֐����t�@�����X�i�[��
     *  @retval     true        ����
     */
    bool GetLuaFunctionReference(const std::string& param_name, int32_t& o_ref);
    /*!
     *  @brief  lua�֐��Ăяo��
     *  @param  func_ref    lua�֐����t�@�����X
     *  @param              lua�֐������Q
     *  @return ���������_�p�����[�^
     */
    bool CallLuaBoolFunction(int32_t func_ref, float64, float64, float64, float64);
    float64 CallLuaFloatFunction(int32_t func_ref, float64, float64, float64, float64);

private:
    LuaAccessor(const LuaAccessor&);
    LuaAccessor& operator= (const LuaAccessor&);

    //!< Lua�X�e�[�g(��p�̏I��������ʂ��K�v������̂Ő��|�c)
    lua_State* m_pState;
    //!< Lua�e�[�u�����J�������̃X�^�b�N�ʒu
    std::stack<int32_t> m_table_top_stack;
};
