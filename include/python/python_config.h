/*!
 *  @file   python_config.h
 *  @brief  [common]python�ݒ�
 *  @date   2018/01/14
 *  @note   boost �Ɉˑ����Ă�
 */
#pragma once

#include "python_define.h"
#include <string>

namespace garnet
{
/*!
 *  @brief  python�ݒ�
 *  @note   ini�t�@�C����ǂݍ���ŊO���ɒ�
 */
class python_config
{
public:
    /*!
     *  @brief  �󐶐�
     */
    python_config();
    /*!
     *  @param  ini�t�@�C����
     */
    python_config(const std::string& ini_file_name);

    /*!
     *  @brief  python�̃C���X�g�[���p�X(full)�𓾂�
     *  @return �C���X�g�[���p�X�|�C���^
     *  @note   �߂�l����const���|�Ȃ̂�Py_SetPythonHome�������v�����邽�߁c
     *  @note   �^��python2.X�n�ȑO��python3.X�ȍ~�ňقȂ�
     */
    PYCHAR* GetPythonHome() const
    {
        return const_cast<PYCHAR*>(m_install_path.c_str());
    }
    /*!
     *  @brief  python�X�N���v�g�p�X�𓾂�
     */
    const std::string& GetPythonScriptPath() const
    {
        return m_script_path;
    }

private:
    python_config(const python_config&);
    python_config(python_config&&);
    python_config& operator= (const python_config&);

    /*!
     *  @brief  ������
     *  @param  ini�t�@�C����
     */
    void initialize(const std::string& ini_file_name);

    PYSTRING m_install_path;    //!< python�C���X�g�[���p�X
    std::string m_script_path;  //!< python�X�N���v�g�p�X
};

} // namespace garnet
