/*!
 *  @file   win_main_struct.h
 *  @brief  win_main���֐��Ԃł��Ƃ�static�ϐ����܂Ƃ߂�\����
 *  @note   WinMain�N���X������Ē��Ŋ���������̂������H
 *  @date   2017/05/08
 */
#pragma once

#include <memory>

namespace trading { class TradeAssistor; }

struct WinMainStruct
{
    enum {
        MAX_LOADSTRING = 100
    };

    HINSTANCE m_hInstance;  				//!< ���݂̃C���^�[�t�F�C�X
    TCHAR m_szTitle[MAX_LOADSTRING];		//!< �^�C�g�� �o�[�̃e�L�X�g
    TCHAR m_szWindowClass[MAX_LOADSTRING];  //!< ���C�� �E�B���h�E �N���X��

    std::weak_ptr<trading::TradeAssistor>   m_TradeAssistor;    //!< �g���[�h�⏕

    WinMainStruct();
    ~WinMainStruct();
};
