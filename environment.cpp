/*!
*  @file   environment.cpp
*  @brief  ���ݒ�
*  @date   2017/12/11
*/
#include "environment.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include "boost/optional.hpp"

#if defined(PYTHON_USE_WCHAR)
#include <codecvt>
#endif/* PYTHON_USE_WCHAR */

std::weak_ptr<Environment> Environment::m_pInstance;

/*!
 *  @brief  �C���X�^���X����
 *  @return �C���X�^���X���L�|�C���^
 */
std::shared_ptr<Environment> Environment::Create(void)
{
    if (m_pInstance.lock()) {
        std::shared_ptr<Environment> _empty_instance;
        return _empty_instance;
    } else {
        std::shared_ptr<Environment> _instance(new Environment());
        m_pInstance = _instance;

        _instance->Initialize();

        return _instance;
    }
}

/*!
 *  @brief
 */
Environment::Environment()
: m_python_home()
, m_trading_script()
{
}

/*!
 *  @brief  ������
 */
void Environment::Initialize()
{
    boost::property_tree::ptree pt;
    boost::property_tree::read_ini("trade_assistant.ini", pt);

    {
        boost::optional<std::string> str = pt.get_optional<std::string>("Path.PythonHome");
#if defined(PYTHON_USE_WCHAR)
        std::string python_work(str.get());
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utfconv;
        m_python_home = utfconv.from_bytes(python_work);
#else
        m_python_home = str.get();
#endif/* PYTHON_USE_WCHAR */
    }
    {
        boost::optional<std::string> str = pt.get_optional<std::string>("Script.TradingScript");
        m_trading_script = str.get();
    }
}
