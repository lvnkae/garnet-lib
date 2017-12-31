/*!
 *  @file   environment.h
 *  @brief  ���ݒ�
 *  @date   2017/12/11
 *  @note   ini�t�@�C������
 */
#pragma once

#include <memory>
#include <string>

class Environment;

/*!
 *  @note   singleton
 *  @note   �C���X�^���X�͖����I�ɐ�������(�ďo����shared_ptr�ŕێ�)
 *  @note   "����C���X�^���X�擾������"�ł͂Ȃ�
 */
class Environment
{
public:
    /*!
     *  @brief  �C���X�^���X���� + ������
     *  @return �C���X�^���X���L�|�C���^
     *  @note   �߂���shared_ptr���L���ȊԂ͋�shared_ptr��Ԃ�
     */
    static std::shared_ptr<Environment> Create();
    /*!
     *  @brief  �C���X�^���X�擾
     *  @return �C���X�^���X��Q��(const)
     *  @note   �����O�ɌĂ΂ꂽ���weak_ptr��Ԃ�
     */
    static std::weak_ptr<const Environment> GetInstance() { return m_pInstance; }

    /*!
     *  @brief  �g���[�f�B���O�X�N���v�g���𓾂�
     */
    std::string GetTradingScript() const { return m_trading_script; }

    /*!
     *  @brief  Python�̃C���X�g�[���p�X(full)�𓾂�
     */
    static PYCHAR* GetPythonHome() {
        std::shared_ptr<const Environment> p = Environment::GetInstance().lock();
        if (nullptr == p) {
            return nullptr;
        }
        //  �߂�l����const�Ȃ̂�Py_SetPythonHome�������v�����邽�߁c
        return const_cast<PYCHAR*>(p->m_python_home.c_str());
    }

private:
    Environment();
    Environment(const Environment&);
    Environment& operator= (const Environment&);

    /*!
     *  @brief  ������
     */
    void Initialize();

    PYSTRING m_python_home;         //! Python�C���X�g�[���p�X
    std::string m_trading_script;   //! �g���[�f�B���O�X�N���v�g��

    static std::weak_ptr<Environment> m_pInstance; //! ���g�̎�Q��
};
