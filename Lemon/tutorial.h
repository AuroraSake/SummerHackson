//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//struct TITLE
//{
//	//XMFLOAT3	pos;		// �|���S���̍��W
//	float		w, h;		// ���ƍ���
//	int			texNo;		// �g�p���Ă���e�N�X�`���ԍ�
//};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

