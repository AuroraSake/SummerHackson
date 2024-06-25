//=============================================================================
//
// �o���b�g���� [effect.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EFFECT_MAX		(15)		// �o���b�g��Max��
#define EFFECT_SPEED	(200.0f)		// �o���b�g�̈ړ��X�s�[�h
#define EFFECT_SPAN		(100)


// �o���b�g�\����
struct EFFECT
{
	BOOL				use;				// true:�g���Ă���  false:���g�p
	float				w, h;				// ���ƍ���
	XMFLOAT3			pos;				// �o���b�g�̍��W
	XMFLOAT3			rot;				// �o���b�g�̉�]��
	int					span;				//���ݎ���
	int					texNo;				// ���Ԗڂ̃e�N�X�`���[���g�p����̂�
	int					countAnim;			// �A�j���[�V�����J�E���g
	int					patternAnim;		// �A�j���[�V�����p�^�[���i���o�[
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT   InitEffect(void);
void    UninitEffect(void);
void    UpdateEffect(void);
void      DrawEffect(void);

void SetEffect(XMFLOAT3 pos);
