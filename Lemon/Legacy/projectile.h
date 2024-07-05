//=============================================================================
//
// Projectile���� [projectile.h]
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
#define PROJECTILE_MAX		(100)			// Projectile��Max��
#define PROJECTILE_SPEED	(10.0f)		// �o���b�g�̈ړ��X�s�[�h

#define KILL_Y 50	//��ʉ�����ǂ�ʂ̂������Z���ꂽ��Projectile�����g�p�ɂ���邩

#define GRAVITY 0.163f			//�d�͒萔 9.8/60frame
#define GRAVITY_ADDITIVE 0.1f	//�d�͉����x�I�ȕ�


// Projectile�\����
struct PROJECTILE
{
	BOOL				use;				// true:�g���Ă���  false:���g�p
	float				w, h;				// ���ƍ���
	XMFLOAT3			pos;				// �o���b�g�̍��W
	XMFLOAT3			rot;				// �o���b�g�̉�]��
	XMFLOAT3			move;				// �o���b�g�̈ړ���
	float				rotRate;			// ����X�s�[�h
	int					texNo;				// ���Ԗڂ̃e�N�X�`���[���g�p����̂�

};

struct CUT
{
	PROJECTILE			prj;
	BOOL				useFreeShape;
    int                 span;
    //XMFLOAT4           *vertices;
    XMFLOAT4            vertices[6];
    int                 vertexNum;
};


enum
{
	FRUIT_PURPLE,
	FRUIT_LEMON,
	FRUIT_PEAR,
	FRUIT_BANANA,
	FRUIT_HABANERO,
	FRUIT_ULT,
	FRUIT_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitProjectile(void);
void UninitProjectile(void);
void UpdateProjectile(void);
void DrawProjectile(void);

PROJECTILE *GetProjectile(void);
void  SetProjectile(XMFLOAT3 pos, XMFLOAT3 move, float size, int texNo, float rotZ, int score);

void RandomSpawn(void);

void ResetInterval(void);
void CutProjectile(int projectileID, float rot, XMFLOAT3 pos);

void SetCut(PROJECTILE projectile, float rot, BOOL upperPart);

XMFLOAT4 ApproxyVertexParam(PROJECTILE parent, int index);
float normalize(float rot, float rot_base);
float regulate(float rot);
float regulateA(float rot);

void SetCutFreeShape(PROJECTILE parent, float rot, XMFLOAT3 pos);
void SetCutFreeRot(PROJECTILE parent, float rot);

XMFLOAT4 checkExtremum (XMFLOAT2 pos, XMFLOAT4 extremums);