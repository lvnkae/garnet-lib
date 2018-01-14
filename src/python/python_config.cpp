/*!
*  @file   python_config.cpp
*  @brief  [common]python�ݒ�
*  @date   2017/12/11
*/
#include "python/python_config.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include "boost/optional.hpp"

#include <fstream>
#if defined(PYTHON_USE_WCHAR)
#include <codecvt>
#endif/* PYTHON_USE_WCHAR */

namespace garnet
{

/*!
 *  @brief  �󐶐�
 */
python_config::python_config()
: m_install_path()
, m_script_path()
{
}

/*!
 *  @param  ini�t�@�C����
 */
python_config::python_config(const std::string& ini_file_name)
: m_install_path()
, m_script_path()
{
    initialize(ini_file_name);
}

/*!
 *  @brief  ������
 *  @param  ini�t�@�C����
 */
void python_config::initialize(const std::string& ini_file_name)
{
    std::ifstream ifs(ini_file_name);
    if (!ifs.is_open()) {
        return; // ini�t�@�C�����Ȃ�
    }
    boost::property_tree::ptree pt;
    boost::property_tree::read_ini(ini_file_name, pt);

    {
        boost::optional<std::string> str = pt.get_optional<std::string>("Path.PythonHome");
#if defined(PYTHON_USE_WCHAR)
        std::string python_home(str.get());
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utfconv;
        m_install_path = utfconv.from_bytes(python_home);
#else
        m_install_path = str.get();
#endif/* PYTHON_USE_WCHAR */
    }
    {
        boost::optional<std::string> str = pt.get_optional<std::string>("Path.PythonScript");
        m_script_path = str.get();
    }
}

} // namespace garnet
