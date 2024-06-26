//=============================================================================
//
// バレット処理 [Effect.cpp]
// Author : 
//
//=============================================================================
#include "effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(100)	// キャラサイズ
#define TEXTURE_HEIGHT				(100)	// 
#define TEXTURE_MAX					(1)		// テクスチャの数


#define TEXTURE_PATTERN_DIVIDE_X	(13)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(4)		// アニメーションの切り替わるWait値

#define PI							(3.14159265358979f)

//#define Effect_DV		(1)	// バレットの移動スピード

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {
	"data/TEXTURE/smoke00.png",
};

static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ
static EFFECT	g_Effect[EFFECT_MAX];	// effect構造体

static float g_Effect_T[EFFECT_MAX];
static XMFLOAT3 SpawnLoc[EFFECT_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(void)
{
	ID3D11Device *pDevice = GetDevice();

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


	// バレット構造体の初期化
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		g_Effect[i].use   = FALSE;			// 未使用（発射されていない弾）
		g_Effect[i].w     = TEXTURE_WIDTH;
		g_Effect[i].h     = TEXTURE_HEIGHT;
		g_Effect[i].pos   = XMFLOAT3(300, 300.0f, 0.0f);
		g_Effect[i].rot   = XMFLOAT3(0.0f, 0.0f, PI/2.0f);
		g_Effect[i].texNo = 0;

		//g_Effect[i].countAnim = 0;
		//g_Effect[i].patternAnim = 0;

		//g_Effect[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);	// 移動量を初期化
	}
	
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	if (g_Load == FALSE) return;

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
void UpdateEffect(void)
{
	int EffectCount = 0;				// 処理したバレットの数



	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == TRUE)	// このバレットが使われている？
		{								// Yes

			g_Effect[i].span--;

			// アニメーション  
			g_Effect[i].countAnim++;
			if ((g_Effect[i].countAnim % ANIM_WAIT) == 0)
			{
				// パターンの切り替え
				g_Effect[i].patternAnim = (g_Effect[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}

			if(g_Effect[i].span < 1)
				g_Effect[i].use = FALSE;

			EffectCount++;
		}
	}


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
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
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	SetBlendState(BLEND_MODE_ADD);		// 加算合成

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == TRUE)		// このバレットが使われている？
		{									// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Effect[i].texNo]);

			//バレットの位置やテクスチャー座標を反映
			float px = g_Effect[i].pos.x;	// バレットの表示位置X
			float py = g_Effect[i].pos.y;	// バレットの表示位置Y
			float pw = g_Effect[i].w;		// バレットの表示幅
			float ph = g_Effect[i].h;		// バレットの表示高さ

            int frame = (int)(g_Effect[i].span * 0.12f);

			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
			float th = 1.0f;	// テクスチャの高さ
			float tx = (float)(frame % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
			float ty = 0.0f;	// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer, 
				px, py, pw, ph, 
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.75f),
				g_Effect[i].rot.z);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	SetBlendState(BLEND_MODE_ALPHABLEND);	// 半透明処理を元に戻す

}


//=============================================================================
// バレット構造体の先頭アドレスを取得
//=============================================================================


//=============================================================================
// バレットの発射設定
//=============================================================================
void SetEffect(XMFLOAT3 pos)
{
	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == FALSE)		// 未使用状態のバレットを見つける
		{
			g_Effect[i].use = TRUE;			// 使用状態へ変更する
			g_Effect[i].pos = pos;			// 座標をセット
			g_Effect[i].span = EFFECT_SPAN;

			return;							// 1発セットしたので終了する
		}
	}
}
