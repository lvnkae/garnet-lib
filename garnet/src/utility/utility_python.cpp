/*!
 *  @file   utility_python.cpp
 *  @brief  [common]python���݂�Utility�֐�
 *  @date   2017/05/13
 */
#include "utility_python.h"

#include "python_config.h"
#include "utility_debug.h"
#include <fstream>

namespace garnet
{
namespace utility_python
{

/*!
 *  @brief  python�̃X�N���v�g���g����悤�ɂ���
 *  @param  python_home     python�ݒ�
 *  @param  python_script   python�̃X�N���v�g�t�@�C����
 */
boost::python::api::object PreparePythonScript(const python_config_ref& config, const std::string& python_script)
{
    const python_config_ptr p = config.lock();
    if (nullptr == p) {
        return boost::python::api::object();
    }
    return PreparePythonScript(p->GetPythonHome(),
                               p->GetPythonScriptPath() + python_script);
}

/*!
 *  @brief  python�̃X�N���v�g���g����悤�ɂ���
 *  @param  python_home     python�̃C���X�g�[���p�X(full)
 *  @param  python_script   python�̃X�N���v�g�t�@�C����
 */
boost::python::api::object PreparePythonScript(PYCHAR* python_home, const std::string& python_script)
{
    if (nullptr == python_home) {
        return boost::python::api::object();
    }

    Py_SetPythonHome(python_home);
    Py_Initialize();
    boost::python::api::object main_mod = boost::python::import("__main__");
    boost::python::api::object_attribute main_ns = main_mod.attr("__dict__");
    // read python script
    std::ifstream ifs(python_script.c_str());
    std::string script((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    // execute python script,
    // but not invoke timeline() function (definition only)
    boost::python::exec(script.c_str(), main_ns);
    //
    return main_mod;
}


/*!
 *  @brief  python�X�N���v�g�G���[�o��
 *  @param  e   �G���[
 */
void OutputPythonError(boost::python::error_already_set& e)
{
    PyObject* extype = nullptr;
    PyObject* value = nullptr;
    PyObject* traceback = nullptr;
    PyErr_Fetch(&extype, &value, &traceback);
    if (nullptr == extype) {
        return;
    }

    boost::python::object o_extype(boost::python::handle<>(boost::python::borrowed(extype)));
    boost::python::object o_value(boost::python::handle<>(boost::python::borrowed(value)));
    boost::python::object o_traceback(boost::python::handle<>(boost::python::borrowed(traceback)));

    boost::python::object mod_traceback = boost::python::import("traceback");
    boost::python::object lines = mod_traceback.attr("format_exception")(o_extype, o_value, o_traceback);

    std::string error_str;
    for (int32_t i = 0; i < boost::python::len(lines); i++) {
        error_str += boost::python::extract<std::string>(lines[i])();
    }

    utility_debug::DebugOutput(error_str);
}

} // namespace utility_python
} // namespace garnet
