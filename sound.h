//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
enum 
{
	SOUND_LABEL_SE_cut,		// 
	SOUND_LABEL_SE_decision,		// 
	SOUND_LABEL_SE_whiff,		// 


	SOUND_LABEL_BGM_title,		// BGM Maou	// BGM Maou
	SOUND_LABEL_BGM_game,		// BGM Maou	// BGM Maou

	SOUND_LABEL_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

