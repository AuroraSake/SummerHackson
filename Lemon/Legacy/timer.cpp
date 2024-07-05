/*******************************************************************************
* タイトル:		タイマープログラム
* プログラム名:	timer.cpp
* 作成者:		GP11A132 21 野寺翔悟
* 作成日:		2023/07/28
*******************************************************************************/



/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "main.h"
#include "timer.h"
#include "iostream"
#include "vector"
#include "windows.h"
#include "renderer.h"
#include "sprite.h"
#pragma comment(lib, "winmm.lib")

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define TEXTURE_WIDTH				(16)	// キャラサイズ
#define TEXTURE_HEIGHT				(32)	// 
#define TEXTURE_MAX					(1)		// テクスチャの数

/*******************************************************************************
* 構造体定義
*******************************************************************************/


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/


/*******************************************************************************
* グローバル変数
*******************************************************************************/
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[] = {
	"data/TEXTURE/number_score.png",
};

int curtime;

static bool						g_Use;						// true:使っている  false:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

static int						g_Timer;					// タイマー

DWORD g_timerStart;


//int maxtimer = TIMER_MAX;
/*******************************************************************************
 関数名:	
 引数　:	
 戻り値:	
 説明　:	
*******************************************************************************/


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTimer(void)
{
	ID3D11Device* pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// プレイヤーの初期化
	g_Use = true;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = { 500.0f, 20.0f, 0.0f };
	g_TexNo = 0;
	curtime = TIMER_MAX;

	return S_OK;


}


//=============================================================================
// 終了処理
//=============================================================================
void UninitTimer(void)
{
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimer(void)
{

	if ((int)timeGetTime() - (int)g_timerStart >= 1 * 1000)//ミリ秒にするのに1000倍
	{
		g_timerStart = timeGetTime();
		//curtime = maxtimer - (g_timerStart / 1000);
		curtime--;
	}


#ifdef _DEBUG	// デバッグ情報を表示する
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}


//=============================================================================
// 描画処理
//=============================================================================
void DrawTimer(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
    //if curtime <
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);


	// 桁数分処理する
	int number = curtime;
	//number = 20;
	for (int i = 0; i < TIMER_DIGIT; i++)
	{
		// 今回表示する桁の数字
		float x = number;
		//float x = 19;

		// スコアの位置やテクスチャー座標を反映
		float px = g_Pos.x - g_w * i;	// スコアの表示位置X
		float py = g_Pos.y;			// スコアの表示位置Y
		float pw = g_w;				// スコアの表示幅
		float ph = g_h;				// スコアの表示高さ

		float tw = 1.0f / 10;		// テクスチャの幅
		float th = 1.0f / 1;		// テクスチャの高さ
		float tx = 0;// = x * tw;			// テクスチャの左上X座標 
		//if (i == 0)
		//{
			tx = (int)x % 10 * tw;
		//}
		//else if (i == 1)
		//{
		//	//tx = (int)x % 10 * tw;
		//	tx = x / 10 * tw;
		//}
		float ty = 0.0f;			// テクスチャの左上Y座標
        XMFLOAT4 diffuse = {1.0f, 1.0f, 1.0f, 1.0f};
        if (curtime < 10)
            diffuse = {1.0f, 0.0f, 0.0f, 1.0f};


		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
            diffuse);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		// 次の桁へ
		number /= 10;
	}

}


//=============================================================================
// タイマーを減算する
// 引数:add :追加する点数。マイナスも可能
//=============================================================================


int GetTimer(void)
{
	return curtime;
}


void PassTime(int time)
{
	curtime -= time;
	if (curtime <= 0)
	{
		curtime = 0;
	}
}