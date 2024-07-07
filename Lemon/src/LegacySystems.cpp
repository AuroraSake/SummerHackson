#pragma  once
#include "LegacySystems.h"
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


static int g_Mode;

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

void Update()
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


void Draw()
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

    DrawFade();

#ifdef _DEBUG
    // �f�o�b�O�\��
    DrawDebugProc();
#endif

    // �o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
    Present();
}


void Init(const HINSTANCE& instance, const HWND& hWnd, bool isWindowed, int defaultMode)
{
    InitRenderer(instance, hWnd, isWindowed);
    SetCullingMode(CULL_MODE_BACK);
    SetLightEnable(FALSE);

    InitCamera();
    InitFade();

    InitSound(hWnd);
    InitInput(instance, hWnd);

    SetMode(defaultMode);
}

void Uninit()
{
    SetMode(MODE_MAX);

    UninitCamera();
    UninitFade();

    UninitSound();
    UninitInput();

    UninitRenderer();
}

int GetMode()
{
    return g_Mode;
}