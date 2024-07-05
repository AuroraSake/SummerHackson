//=============================================================================
//
// メイン処理 [main.cpp]
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
// グローバル変数:
//*****************************************************************************
long g_MouseX = 0;
long g_MouseY = 0;


#ifdef _DEBUG
int		g_CountFPS;						// FPSカウンタ
char	g_DebugStr[2048] = "temp";		// デバッグ文字表示用
int	g_Mode = MODE_GAME;					// 起動時の画面を設定

#else
int	g_Mode = MODE_TITLE;					// 起動時の画面を設定
#endif


//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// 時間計測用
    Window window(hInstance, nCmdShow);
    if (!window.CreatWindow(NULL))
        return -1;
    window.Show();


	// 終了処理
	Uninit();
    return 0;
}

//=============================================================================
// プロシージャ
//=============================================================================

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// 描画の初期化
	InitRenderer(hInstance, hWnd, bWindow);

	// カメラの初期化
	InitCamera();

	//フェードの初期化
	InitFade();

	// ライトを無効化
	SetLightEnable(FALSE);

	// 背面ポリゴンをカリング
	SetCullingMode(CULL_MODE_BACK);

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	//サウンドの初期化
	InitSound(hWnd);

	// 最初のモードをセット
	SetMode(g_Mode);	// ここはSetModeのままで！

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	// 終了のモードをセット
	SetMode(MODE_MAX);

	//フェードの終了処理
	UninitFade();

	//サウンドの終了処理
	UninitSound();

	// 入力の終了処理
	UninitInput();

	// カメラの終了処理
	UninitCamera();

	// レンダラーの終了処理
	UninitRenderer();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
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

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
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

	//Fade処理
	DrawFade();
 
#ifdef _DEBUG
	// デバッグ表示
	DrawDebugProc();
#endif

	// バックバッファ、フロントバッファ入れ替え
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
// モードの設定
//=============================================================================
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

//=============================================================================
// モードの取得
//=============================================================================
int GetMode(void)
{
	return g_Mode;
}
