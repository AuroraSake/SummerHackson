//=============================================================================
//
// ���C������ [main.h]
// Author : 
//
//=============================================================================
#pragma once
#include "src/LegacySystems.h"

#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~
#include <stdio.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>


// �{���̓w�b�_�ɏ����Ȃ������ǂ�
using namespace DirectX;


#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
#include "dinput.h"
#include "mmsystem.h"

#include "myMath.h"

#pragma warning(pop)

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment (lib, "d3d11.lib")		
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")	
#pragma comment (lib, "d3dx9.lib")	
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH	(960)			// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(540)			// �E�C���h�E�̍���
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// �E�C���h�E�̒��S�x���W

#define	MAP_W			(1280.0f)
#define	MAP_H			(1280.0f)
#define	MAP_TOP			(MAP_H/2)
#define	MAP_DOWN		(-MAP_H/2)
#define	MAP_LEFT		(-MAP_W/2)
#define	MAP_RIGHT		(MAP_W/2)


#define DIK_ANY         0xFF
#define Float_Percision 0.001f

// ��ԗp�̃f�[�^�\���̂��`
struct INTERPOLATION_DATA
{
	XMFLOAT3	pos;		// ���_���W
	XMFLOAT3	rot;		// ��]
	XMFLOAT3	scl;		// �g��k��
	float		frame;		// ���s�t���[���� ( dt = 1.0f/frame )
};

enum
{
	MODE_TITLE = 0,			// �^�C�g�����
	MODE_TUTORIAL,			// �Q�[���������
	MODE_GAME,				// �Q�[�����
	MODE_RESULT,			// ���U���g���
	MODE_MAX
};

//�S�Ń`�F�b�N
int CheckGameClear(void);