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
    // モードを変える前に全部メモリを解放しちゃう
    StopSound();

    // タイトル画面の終了処理
    UninitTitle();

    // 背景画面の終了処理
    UninitBG();

    UninitResult();
    UninitTutorial();

    UninitPlayer();
    UninitProjectile();
    UninitBullet();
    UninitEffect();
    UninitScore();
    UninitTimer();

    g_Mode = mode;	// 次のモードをセットしている

    switch (g_Mode)
    {
    case MODE_TITLE:
        // タイトル画面の初期化
        InitTitle();
        PlaySound(SOUND_LABEL_BGM_title);
        break;
    case MODE_TUTORIAL:
        // タイトル画面の初期化
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
    // 入力の更新処理
    UpdateInput();

    // カメラ更新
    UpdateCamera();

    int clear = 0;

    // モードによって処理を分ける
    switch (g_Mode)
    {
    case MODE_TITLE:		// タイトル画面の更新
        UpdateTitle();
        break;
    case MODE_TUTORIAL:
        // タイトル画面の初期化
        UpdateTutorial();
        break;
    case MODE_GAME:			// ゲーム画面の更新
        UpdateBG();
        UpdatePlayer();
        UpdateProjectile();
        UpdateBullet();
        UpdateEffect();
        UpdateScore();
        UpdateTimer();

        //エネミー全滅チェック
        //clear = GetMode();
        if (GetTimer() <= 0)
        {
            //SetMode(MODE_RESULT);
            SetFade(FADE_OUT, MODE_RESULT);
        }
        break;

    case MODE_RESULT:		// リザルト画面の更新
        UpdateResult();
        break;
    }

    //Fade処理
    UpdateFade();
}


void Draw()
{
    // バックバッファクリア
    Clear();

    SetCamera();

    // 2Dの物を描画する処理
    SetViewPort(TYPE_FULL_SCREEN);

    // Z比較なし
    SetDepthEnable(FALSE);

    // ライティングを無効
    SetLightEnable(FALSE);


    // モードによって処理を分ける
    switch (g_Mode)
    {
    case MODE_TITLE:		// タイトル画面の描画
        DrawTitle();
        break;
    case MODE_TUTORIAL:
        // タイトル画面の初期化
        DrawTutorial();
        break;
    case MODE_GAME:			// ゲーム画面の描画
        DrawBG();
        DrawPlayer();
        DrawProjectile();
        DrawBullet();
        DrawEffect();
        DrawScore();
        DrawTimer();
        break;

    case MODE_RESULT:		// リザルト画面の描画
        DrawResult();
        break;
    }

    DrawFade();

#ifdef _DEBUG
    // デバッグ表示
    DrawDebugProc();
#endif

    // バックバッファ、フロントバッファ入れ替え
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