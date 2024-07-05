//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
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
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

