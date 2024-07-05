//=============================================================================
//
// ���C������ [main.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "debugproc.h"
#include "input.h"
#include "bg.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "effect.h"
#include "title.h"
#include "score.h"
#include "result.h"
#include "sound.h"
#include "fade.h"
#include "projectile.h"
#include "timer.h"
#include "tutorial.h"

#include "Lemon/Window.h"


HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
long g_MouseX = 0;
long g_MouseY = 0;


#ifdef _DEBUG
int		g_CountFPS;						// FPS�J�E���^
char	g_DebugStr[2048] = "temp";		// �f�o�b�O�����\���p
int	g_Mode = MODE_GAME;					// �N�����̉�ʂ�ݒ�

#else
int	g_Mode = MODE_TITLE;					// �N�����̉�ʂ�ݒ�
#endif


//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// ���Ԍv���p
    Window window(hInstance, nCmdShow);
    if (!window.CreatWindow(NULL))
        return -1;
    window.Show();


	// �I������
	Uninit();
    return 0;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �`��̏�����
	InitRenderer(hInstance, hWnd, bWindow);

	// �J�����̏�����
	InitCamera();

	//�t�F�[�h�̏�����
	InitFade();

	// ���C�g�𖳌���
	SetLightEnable(FALSE);

	// �w�ʃ|���S�����J�����O
	SetCullingMode(CULL_MODE_BACK);

	// ���͏����̏�����
	InitInput(hInstance, hWnd);

	//�T�E���h�̏�����
	InitSound(hWnd);

	// �ŏ��̃��[�h���Z�b�g
	SetMode(g_Mode);	// ������SetMode�̂܂܂ŁI

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	// �I���̃��[�h���Z�b�g
	SetMode(MODE_MAX);

	//�t�F�[�h�̏I������
	UninitFade();

	//�T�E���h�̏I������
	UninitSound();

	// ���͂̏I������
	UninitInput();

	// �J�����̏I������
	UninitCamera();

	// �����_���[�̏I������
	UninitRenderer();
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	// ���͂̍X�V����
	UpdateInput();

	// �J�����X�V
	UpdateCamera();
	
	int clear = 0; 

	// ���[�h�ɂ���ď����𕪂���
	switch (g_Mode)
	{
	case MODE_TITLE:		// �^�C�g����ʂ̍X�V
		UpdateTitle();
		break;
	case MODE_TUTORIAL:
		// �^�C�g����ʂ̏�����
		UpdateTutorial();
		break;
	case MODE_GAME:			// �Q�[����ʂ̍X�V
		UpdateBG();
		UpdatePlayer();
		UpdateProjectile();
		UpdateBullet();
		UpdateEffect();
		UpdateScore();
		UpdateTimer();

		//�G�l�~�[�S�Ń`�F�b�N
		//clear = GetMode();
		if (GetTimer() <= 0)
		{
			//SetMode(MODE_RESULT);
			SetFade(FADE_OUT, MODE_RESULT);
		}
		break;

	case MODE_RESULT:		// ���U���g��ʂ̍X�V
		UpdateResult();
		break;
	}

	//Fade����
	UpdateFade();

}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@�N���A
	Clear();

	SetCamera();

	// 2D�̕���`�悷�鏈��
	SetViewPort(TYPE_FULL_SCREEN);

	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);


	// ���[�h�ɂ���ď����𕪂���
	switch (g_Mode)
	{
	case MODE_TITLE:		// �^�C�g����ʂ̕`��
		DrawTitle();
		break;
	case MODE_TUTORIAL:
		// �^�C�g����ʂ̏�����
		DrawTutorial();
		break;
	case MODE_GAME:			// �Q�[����ʂ̕`��
		DrawBG();
		DrawPlayer();
		DrawProjectile();
		DrawBullet();
		DrawEffect();
		DrawScore();
		DrawTimer();
		break;

	case MODE_RESULT:		// ���U���g��ʂ̕`��
		DrawResult();
		break;
	}

	//Fade����
	DrawFade();
 
#ifdef _DEBUG
	// �f�o�b�O�\��
	DrawDebugProc();
#endif

	// �o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();
}


long GetMousePosX(void)
{
	return g_MouseX;
}


long GetMousePosY(void)
{
	return g_MouseY;
}


#ifdef _DEBUG
char* GetDebugStr(void)
{
	return g_DebugStr;
}
#endif



//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void SetMode(int mode)
{
	// ���[�h��ς���O�ɑS������������������Ⴄ
	StopSound();

	// �^�C�g����ʂ̏I������
	UninitTitle();

	// �w�i��ʂ̏I������
	UninitBG();

	UninitResult();
	UninitTutorial();

	UninitPlayer();
	UninitProjectile();
	UninitBullet();
	UninitEffect();
	UninitScore();
	UninitTimer();

	g_Mode = mode;	// ���̃��[�h���Z�b�g���Ă���

	switch (g_Mode)
	{
	case MODE_TITLE:
		// �^�C�g����ʂ̏�����
		InitTitle();
		PlaySound(SOUND_LABEL_BGM_title);
		break;	
	case MODE_TUTORIAL:
		// �^�C�g����ʂ̏�����
		InitTutorial();
		PlaySound(SOUND_LABEL_BGM_title);
		break;

	case MODE_GAME:
		InitBG();
		InitProjectile();
		InitBullet();
		InitEffect();
		InitScore();
		InitTimer();
		InitPlayer();
		PlaySound(SOUND_LABEL_BGM_game);
		break;

	case MODE_RESULT:
		InitResult();
		PlaySound(SOUND_LABEL_BGM_title);
		break;

	case MODE_MAX:
		break;
	}
}

//=============================================================================
// ���[�h�̎擾
//=============================================================================
int GetMode(void)
{
	return g_Mode;
}
