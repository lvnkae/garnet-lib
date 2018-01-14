/*!
 *  @file   lua_accessor.cpp
 *  @brief  [common]lua�Ƃ̂��Ƃ蒇��
 *  @date   2017/05/04
 */
#include "lua_accessor.h"
#include "lua.hpp"

namespace garnet
{
namespace LuaAccessorLocal
{

const int32_t STACKTOP = -1;

/*!
 *  @brief  �e�[�u���p�����[�^�擾(Core)
 *  @param[in]  param_name  �p�����[�^��
 *  @param[out] o_param     �p�����[�^�i�[��
 *  @retval     true        ����
 *  @note   �J����(�X�^�b�N�ɐς܂ꂽ)��Ԃ̃e�[�u�����p�����[�^�擾
 */
bool GetTableParamCore(lua_State* state_ptr, const std::string& param_name, std::string& o_param)
{
    if (0 == lua_getfield(state_ptr, LuaAccessorLocal::STACKTOP, param_name.c_str())) {
        return false;
    }
    if (!lua_isstring(state_ptr, LuaAccessorLocal::STACKTOP)) {
        return false;
    }
    o_param = lua_tostring(state_ptr, LuaAccessorLocal::STACKTOP);
    return true;
}
bool GetTableParamCore(lua_State* state_ptr, const std::string& param_name, int32_t& o_param)
{
    if (0 == lua_getfield(state_ptr, LuaAccessorLocal::STACKTOP, param_name.c_str())) {
        return false;
    }
    if (!lua_isinteger(state_ptr, LuaAccessorLocal::STACKTOP)) {
        return false;
    }
    o_param = static_cast<int32_t>(lua_tointeger(state_ptr, LuaAccessorLocal::STACKTOP));
    return true;
}
bool GetTableParamCore(lua_State* state_ptr, const std::string& param_name, float32& o_param)
{
    if (0 == lua_getfield(state_ptr, LuaAccessorLocal::STACKTOP, param_name.c_str())) {
        return false;
    }
    if (!lua_isnumber(state_ptr, LuaAccessorLocal::STACKTOP)) {
        return false;
    }
    o_param = static_cast<float32>(lua_tonumber(state_ptr, LuaAccessorLocal::STACKTOP));
    return true;
}
bool GetTableParamCore(lua_State* state_ptr, const std::string& param_name, float64& o_param)
{
    if (0 == lua_getfield(state_ptr, LuaAccessorLocal::STACKTOP, param_name.c_str())) {
        return false;
    }
    if (!lua_isnumber(state_ptr, LuaAccessorLocal::STACKTOP)) {
        return false;
    }
    o_param = static_cast<float64>(lua_tonumber(state_ptr, LuaAccessorLocal::STACKTOP));
    return true;
}
bool GetTableParamCore(lua_State* state_ptr, const std::string& param_name, bool& o_param)
{
    if (0 == lua_getfield(state_ptr, LuaAccessorLocal::STACKTOP, param_name.c_str())) {
        return false;
    }
    if (!lua_isboolean(state_ptr, LuaAccessorLocal::STACKTOP)) {
        return false;
    }
    o_param = (lua_toboolean(state_ptr, LuaAccessorLocal::STACKTOP)) ? true : false;
    return true;
}
bool GetTableParamCore(lua_State* state_ptr, const int32_t& param_inx, std::string& o_param)
{
    int32_t lua_param_inx = param_inx + 1;
    if (0 == lua_geti(state_ptr, LuaAccessorLocal::STACKTOP, lua_param_inx)) {
        return false;
    }
    if (!lua_isstring(state_ptr, LuaAccessorLocal::STACKTOP)) {
        return false;
    }
    o_param = lua_tostring(state_ptr, LuaAccessorLocal::STACKTOP);
    return true;
}
bool GetTableParamCore(lua_State* state_ptr, const int32_t& param_inx, int32_t& o_param)
{
    int32_t lua_param_inx = param_inx + 1;
    if (0 == lua_geti(state_ptr, LuaAccessorLocal::STACKTOP, lua_param_inx)) {
        return false;
    }
    if (!lua_isinteger(state_ptr, LuaAccessorLocal::STACKTOP)) {
        return false;
    }
    o_param = static_cast<int32_t>(lua_tointeger(state_ptr, LuaAccessorLocal::STACKTOP));
    return true;
}

/*!
 *  @brief  �e�[�u���p�����[�^�擾���b�v�֐�
 *  @param[in]  param_name  �p�����[�^��
 *  @param[out] o_param     �p�����[�^�i�[��
 *  @retval     true        ����
 *  @note   "�e�[�u�����J�����X�^�b�N�̃g�b�v�Ƀe�[�u����ς�"
 *  @note   �X�^�b�N�ɐς񂾃p�����[�^���̂ĂĂ����Ȃ��ƃe�[�u�����g�b�v�łȂ��Ȃ��Ă��܂�
 */
template<typename Tname, typename Tout>
bool GetTableParamWrapper(lua_State* state_ptr, const Tname& param_name, Tout& o_param)
{
    int32_t prev_stack_count = lua_gettop(state_ptr);
    bool result = LuaAccessorLocal::GetTableParamCore(state_ptr, param_name, o_param);
    lua_pop(state_ptr, lua_gettop(state_ptr) - prev_stack_count);
    return result;
}

/*!
 *  @brief  lua�֐��Ăяo��
 *  @param  state_ptr   lua�X�e�[�g
 *  @param  func_ref    lua�֐����t�@�����X
 *  @param  args        lua�֐������Q
 */
template<typename... T>
bool CallLuaBoolFunction(lua_State* state_ptr, int32_t func_ref, T... args)
{
    int32_t prev_stack_count = lua_gettop(state_ptr);

    // �֐����X�^�b�N�ɐς�
    lua_rawgeti(state_ptr, LUA_REGISTRYINDEX, func_ref);
    // �������X�^�b�N�ɐς�
    for (auto const& argument : { args... }) {
        lua_pushnumber(state_ptr, argument);
    }
    // �֐��R�[��
    const int NUM_ARGS = static_cast<int>(sizeof...(args));
    const int NUM_RETVAL = 1;
    int ret = lua_pcall(state_ptr, NUM_ARGS, NUM_RETVAL, 0);
    // �X�^�b�N����߂�l�𓾂�
    bool retval = false;
    if (ret == LUA_OK && lua_isboolean(state_ptr, LuaAccessorLocal::STACKTOP)) {
        retval = (lua_toboolean(state_ptr, LuaAccessorLocal::STACKTOP)) ?true :false;
    }

    lua_pop(state_ptr, lua_gettop(state_ptr) - prev_stack_count);
    return retval;
}
template<typename RT, typename... T>
RT CallLuaValueFunction(lua_State* state_ptr, int32_t func_ref, T... args)
{
    int32_t prev_stack_count = lua_gettop(state_ptr);

    // �֐����X�^�b�N�ɐς�
    lua_rawgeti(state_ptr, LUA_REGISTRYINDEX, func_ref);
    // �������X�^�b�N�ɐς�
    for (auto const& argument : { args... }) {
        lua_pushnumber(state_ptr, argument);
    }
    // �֐��R�[��
    const int NUM_ARGS = static_cast<int>(sizeof...(args));
    const int NUM_RETVAL = 1;
    int ret = lua_pcall(state_ptr, NUM_ARGS, NUM_RETVAL, 0);
    // �X�^�b�N����߂�l�𓾂�
    RT retval = static_cast<RT>(0);
    if (ret == LUA_OK && lua_isnumber(state_ptr, LuaAccessorLocal::STACKTOP)) {
        retval = static_cast<RT>(lua_tonumber(state_ptr, LuaAccessorLocal::STACKTOP));
    }

    lua_pop(state_ptr, lua_gettop(state_ptr) - prev_stack_count);
    return retval;
}

} // namespace LuaAccessorLocal



/*!
 *  @brief
 */
LuaAccessor::LuaAccessor()
: m_pState(luaL_newstate())
, m_table_top_stack()
{
}
/*!
 *  @brief
 */
LuaAccessor::~LuaAccessor()
{
}
void LuaAccessor::luaStateDeleter::operator()(lua_State* state_ptr) const
{
    lua_close(state_ptr);
}

/*!
 *  @brief  �t�@�C���ǂݍ���
 */
bool LuaAccessor::DoFile(const std::string& file_name)
{
    return !luaL_dofile(m_pState.get(), file_name.c_str());
}

/*!
 *  @brief  �X�^�b�N��S�N���A����
 */
void LuaAccessor::ClearStack()
{  
    lua_State* state_ptr = m_pState.get();

    int now_top = lua_gettop(state_ptr);
    lua_pop(state_ptr, lua_gettop(state_ptr));
    int now_top2 = lua_gettop(state_ptr);
}


/*!
 *  @brief  �e�e�[�u�����J��
 *  @param  table_name  �e�[�u����
 *  @note   �e�[�u���ɃA�N�Z�X�ł���悤�X�^�b�N�ɐς�ł���(getglobal)
 */
int32_t LuaAccessor::OpenTable(const std::string& table_name)
{
    lua_State* state_ptr = m_pState.get();

    // ���݂̃X�^�b�N�ʒu��ێ�
    m_table_top_stack.push(lua_gettop(state_ptr));
    //
    int32_t ret = lua_getglobal(state_ptr, table_name.c_str());
    if (ret == 0) {
        return -1;
    }
    else {
        return static_cast<int32_t>(lua_rawlen(state_ptr, LuaAccessorLocal::STACKTOP));
    }
}
/*!
 *  @brief  �q�e�[�u�����J��
 *  @param  table_name  �q�e�[�u����
 *  @note   �e�[�u�����e�[�u���ɃA�N�Z�X�ł���悤�X�^�b�N�ɐς�ł���(getfield)
 */
int32_t LuaAccessor::OpenChildTable(const std::string& table_name)
{
    lua_State* state_ptr = m_pState.get();

    // ���݂̃X�^�b�N�ʒu��ێ�
    m_table_top_stack.push(lua_gettop(state_ptr));
    //
    int32_t ret = lua_getfield(state_ptr, LuaAccessorLocal::STACKTOP, table_name.c_str());
    if (ret == 0) {
        return -1;
    }
    else {
        return static_cast<int32_t>(lua_rawlen(state_ptr, LuaAccessorLocal::STACKTOP));
    }
}
/*!
 *  @brief  �q�e�[�u�����J��
 *  @param  table_inx    �q�e�[�u���ԍ�(0�n�܂�)
 *  @note   �e�[�u���������e�[�u���ɃA�N�Z�X�ł���悤�X�^�b�N�ɐς�ł���(getfield)
 */
int32_t LuaAccessor::OpenChildTable(int32_t table_inx)
{
    lua_State* state_ptr = m_pState.get();

    // ���݂̃X�^�b�N�ʒu��ێ�
    m_table_top_stack.push(lua_gettop(state_ptr));
    //
    int32_t lua_tbl_inx = table_inx + 1;
    int32_t ret = lua_geti(state_ptr, LuaAccessorLocal::STACKTOP, lua_tbl_inx);
    if (ret == 0) {
        return -1;
    }
    else {
        return static_cast<int32_t>(lua_rawlen(state_ptr, LuaAccessorLocal::STACKTOP));
    }
}
/*!
 *  @brief  �Ō�ɊJ�����e�[�u�����N���[�Y����
 */
void LuaAccessor::CloseTable()
{
    lua_State* state_ptr = m_pState.get();

    // �e�[�u�����J���Ĉȍ~�̃X�^�b�N������Y��ɂ���
    lua_pop(state_ptr, lua_gettop(state_ptr) - m_table_top_stack.top());
    m_table_top_stack.pop();
}


/*!
 *  @brief  �O���[�o���p�����[�^�擾
 *  @param[in]  param_name  �p�����[�^��
 *  @param[out] o_param     �p�����[�^�i�[��
 */
bool LuaAccessor::GetGlobalParam(const std::string& param_name, std::string& o_param)
{
    lua_State* state_ptr = m_pState.get();

    int32_t ret = lua_getglobal(state_ptr, param_name.c_str());
    if (ret == 0) {
        return false;
    }
    if (!lua_isstring(state_ptr, LuaAccessorLocal::STACKTOP)) {
        return false;
    }
    o_param = lua_tostring(state_ptr, LuaAccessorLocal::STACKTOP);
    return true;
}
bool LuaAccessor::GetGlobalParam(const std::string& param_name, int32_t& o_param)
{
    lua_State* state_ptr = m_pState.get();

    int32_t ret = lua_getglobal(state_ptr, param_name.c_str());
    if (ret == 0) {
        return false;
    }
    if (!lua_isinteger(state_ptr, LuaAccessorLocal::STACKTOP)) {
        return false;
    }
    o_param = static_cast<int32_t>(lua_tointeger(state_ptr, LuaAccessorLocal::STACKTOP));
    return true;
}

/*!
 *  @brief  �e�[�u���p�����[�^�擾
 *  @param[in]  param_name  �p�����[�^��
 *  @param[out] o_param     �p�����[�^�i�[��
 */
bool LuaAccessor::GetTableParam(const std::string& param_name, std::string& o_param) { return LuaAccessorLocal::GetTableParamWrapper(m_pState.get(), param_name, o_param); }
bool LuaAccessor::GetTableParam(const std::string& param_name, int32_t& o_param) { return  LuaAccessorLocal::GetTableParamWrapper(m_pState.get(), param_name, o_param); }
bool LuaAccessor::GetTableParam(const std::string& param_name, float32& o_param) { return  LuaAccessorLocal::GetTableParamWrapper(m_pState.get(), param_name, o_param); }
bool LuaAccessor::GetTableParam(const std::string& param_name, float64& o_param) { return  LuaAccessorLocal::GetTableParamWrapper(m_pState.get(), param_name, o_param); }
bool LuaAccessor::GetTableParam(const std::string& param_name, bool& o_param) { return  LuaAccessorLocal::GetTableParamWrapper(m_pState.get(), param_name, o_param); }
/*!
 *  @brief  �z��p�����[�^�擾
 *  @param[in]  param_inx   �p�����[�^�C���b�N�X(0�n�܂�)
 *  @param[out] o_param     �p�����[�^�i�[��
 */
bool LuaAccessor::GetArrayParam(int32_t param_inx, std::string& o_param) {return LuaAccessorLocal::GetTableParamWrapper(m_pState.get(), param_inx, o_param); }
bool LuaAccessor::GetArrayParam(int32_t param_inx, int32_t& o_param) {return LuaAccessorLocal::GetTableParamWrapper(m_pState.get(), param_inx, o_param); }

/*!
*  @brief  lua�֐����t�@�����X�擾
*  @param[in]  param_name  �p�����[�^��(lua�֐��̊i�[���ꂽ�p�����[�^�̖��O)
*  @param[out] o_ref       �֐����t�@�����X�i�[��
*  @note   lua�֐��A�h���X���擾�E�ێ����Ă����Ă������push����d�g�݂��񋟂���Ă��Ȃ�
*  @note   �֐��A�h���X��"�s�N�Ǘ��̈�"�ɓo�^���A�K�v�ȂƂ��Ɏ��o���Ċ֐����Ăяo���̂��u�����������v�Ƃ̂���
*  @note   ���֐��A�h���X��C���ŕێ����Ă��Ă�lua��GC������ł��Ȃ�(�����������)
*  @note   ��"�s�N�̈�"��lua���Ǘ�����̈�Ȃ̂ŎQ�Ƃ��ێ������A����lua������͐G��Ȃ��̈�Ȃ̂ň��S
*/
bool LuaAccessor::GetLuaFunctionReference(const std::string& param_name, int32_t& o_ref)
{
    lua_State* state_ptr = m_pState.get();

    int32_t ret = lua_getfield(state_ptr, LuaAccessorLocal::STACKTOP, param_name.c_str());
    if (ret == 0) {
        return false;
    }
    if (!lua_isfunction(state_ptr, LuaAccessorLocal::STACKTOP)) {
        return false;
    }
    o_ref = luaL_ref(state_ptr, LUA_REGISTRYINDEX);
    return true;
}
/*!
 *  @brief  lua�֐��Ăяo��
 *  @param  func    �֐��|�C���^
 *  @param  fN      ���������_����
 */
bool LuaAccessor::CallLuaBoolFunction(int32_t func_ref, float64 f1, float64 f2, float64 f3, float64 f4)
{
    return LuaAccessorLocal::CallLuaBoolFunction(m_pState.get(), func_ref, f1, f2, f3, f4);
}
float64 LuaAccessor::CallLuaFloatFunction(int32_t func_ref, float64 f1, float64 f2, float64 f3, float64 f4)
{
    return LuaAccessorLocal::CallLuaValueFunction<float64>(m_pState.get(), func_ref, f1, f2, f3, f4);
}

} // namespace
