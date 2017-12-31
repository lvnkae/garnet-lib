/*!
 *  @file   utility_python.h
 *  @brief  [common]python���݂�Utility�֐�
 *  @date   2017/05/13
 *  @note   boost::python �Ɉˑ����Ă���
 */
#pragma once

#include <string>
#include "boost/python.hpp"

/*!
 *  @brief  python�̃X�N���v�g���g����悤�ɂ���
 *  @param  python_home     python�̃C���X�g�[���p�X(full)
 *  @param  python_script   python�̃X�N���v�g�t�@�C����
 *  @return python�A�N�Z�X�I�u�W�F�N�g
 *  @note   python_home����const���|�Ȃ̂�Py_SetPythonHome�̓s���c
 *  @note   ������static��work�łȂ���NG�Aauto�ϐ���n���Ȃ��悤��
 */
boost::python::api::object PreparePythonScript(PYCHAR* python_home, const std::string& python_script);

/*!
 *  @brief  python�X�N���v�g�G���[�o��
 *  @param  e   �G���[
 *  @note   boost::python��python�������Ă�����O(��ɃX�N���v�g���Ŕ��������G���[)���o�͂���
 */
void OutputPythonError(boost::python::error_already_set& e);
