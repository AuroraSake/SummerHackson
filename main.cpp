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


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"Fruit"		        // �E�C���h�E�̃L���v�V������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
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
int		g_CountFPS;							// FPS�J�E���^
char	g_DebugStr[2048] = WINDOW_NAME;		// �f�o�b�O�����\���p
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
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	//�E�B���h�E�o�^�f�[�^
	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																		// �E�B���h�E�̍����W
		CW_USEDEFAULT,																		// �E�B���h�E�̏���W
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// �E�B���h�E����
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
		NULL,
		NULL,
		hInstance,
		NULL);

    BOOL fullScreen = FALSE;
    // �E�B���h�E���[�h���t���X�N���[�����[�h���̏���

	int id = MessageBox(NULL, "Window���[�h�Ńv���C���܂����H", "�N�����[�h", MB_YESNOCANCEL | MB_ICONQUESTION);
	switch (id)
	{
	case IDYES:		// Yes�Ȃ�Window���[�h�ŋN��
		fullScreen = TRUE;
		break;
	case IDNO:		// No�Ȃ�t���X�N���[�����[�h�ŋN��
		fullScreen = FALSE;	// ���ɂ���ē����Ȃ���������
		break;
	case IDCANCEL:	// CANCEL�Ȃ�I��
	default:
		return -1;
		break;
	}

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, hWnd, fullScreen)))
	{
		return -1;
	}

	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
    while (ShowCursor(FALSE) >= 0);
	
	// ���b�Z�[�W���[�v
	while(1)
	{
		//���C�x���g���ł����H
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�

#ifdef _DEBUG	// �f�o�b�O�ł̎�����FPS��\������
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif

				Update();			// �X�V����
				Draw();				// �`�揈��

#ifdef _DEBUG	// �f�o�b�O�ł̎������\������
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);				// ����\��߂�

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD(lParam);
		g_MouseY = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

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

//=============================================================================
// �S�Ń`�F�b�N
//=============================================================================
int CheckGameClear(void)
{
	//0:�Q�[���p�� 1:�v���C���[�̏��� 2:�G�l�~�[�̏���

	//�G�l�~�[���S�ł��Ă��邩�`�F�b�N

	//ENEMY* projectile = GetEnemy();

	//int enemyCount = 0;
	//for (int i = 0; i < ENEMY_MAX; i++)
	//{
	//	if (projectile[i].use == TRUE)
	//	{
	//		enemyCount++;
	//		break;
	//	}
	//}

	//if (enemyCount <= 0)
	//{
	//	return 1;	//1:�v���C���[�̏���
	//}

	////�v���C�₪�S�ł��Ă��邩�`�F�b�N

	//PLAYER* player = GetPlayer();

	//int playerCount = 0;
	//for (int i = 0; i < PLAYER_MAX; i++)
	//{
	//	if (player[i].use == TRUE)
	//	{
	//		playerCount++;
	//		break;
	//	}
	//}

	//if (playerCount <= 0)
	//{
	//	return 2;	//2:�G�l�~�[�̏���
	//}




	return 0;//0:�Q�[���p��
}
