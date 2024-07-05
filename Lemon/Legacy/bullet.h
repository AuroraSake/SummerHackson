//=============================================================================
//
// �o���b�g���� [bullet.h]
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
#define PI							(3.14159265358979f)
#define BULLET_SPEED	            (100.0f)		// �o���b�g�̈ړ��X�s�[�h


// �o���b�g�\����
struct BULLET
{
	BOOL				use;				// true:�g���Ă���  false:���g�p
	float				w, h;				// ���ƍ���
	XMFLOAT3			pos;				// �o���b�g�̍��W
	XMFLOAT3			iPos;		    	// �o���b�g���������̍��W
	XMFLOAT3			rot;				// �o���b�g�̉�]��
	int					span;				//���ݎ���
	int					texNo;				// ���Ԗڂ̃e�N�X�`���[���g�p����̂�
	int					countAnim;			// �A�j���[�V�����J�E���g
	int					patternAnim;		// �A�j���[�V�����p�^�[���i���o�[
};

//XMFLOAT3			move;				// �o���b�g�̈ړ���


struct SLASH
{
	BOOL				use;				// true:�g���Ă���  false:���g�p
	float				w, h;				// ���ƍ���
	XMFLOAT3			pos;				// �o���b�g�̍��W
	int					span;				// �o���b�g�̍��W
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

BULLET *GetBullet(void);
void SetBullet(XMFLOAT3 pos);
void SetBulletFreeRotate(XMFLOAT3 posStart, XMFLOAT3 posEnd);
void SetSlash(XMFLOAT3 pos);


XMVECTOR Bezier(XMVECTOR V0, XMVECTOR V1, float t);

BOOL isInBound(XMFLOAT3 pos, XMFLOAT4 bound);
XMFLOAT3 rayCast(float rot, XMFLOAT3 anker);
XMFLOAT3 rayCastH(float rot, XMFLOAT3 anker);
BOOL hitScan(float rot, XMFLOAT3 anker);
BOOL applyHit(XMFLOAT3 pos, int index, float rot);