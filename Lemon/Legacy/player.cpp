//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "bg.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "score.h"
#include "projectile.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#define CURSORSIZE   				(30)	// キャラサイズ
#define TEXTURE_MAX					(1)		// テクスチャの数


#define PLAYER_MAX		                    (1)		// プレイヤーのMax人数
#define ATTACK_COOLDOWN                     (10)
#define CURSORSPEED                         (10.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {
    "data/TEXTURE/cursor.png",
};


static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ

static XMFLOAT3 slicePosStart;
static XMFLOAT3 slicePosEnd;
static XMFLOAT3 CursorPos;
static XMFLOAT3 oldMousePos;
static XMFLOAT3 oldRightStick;

static float mouseX;
static float mouseY;

static BOOL start;

static int AttCounter = ATTACK_COOLDOWN;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
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


    CursorPos = { SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0};
    mouseX = 0;
    mouseY = 0;
    oldMousePos = { mouseX, mouseY, 0.0 };

    g_Load = TRUE;
    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
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

    g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{

	if(AttCounter > 0)
		AttCounter--;


    mouseX = 0;
    mouseY = 0;

    CursorPos.x += mouseX - oldMousePos.x;
    CursorPos.y += mouseY - oldMousePos.y;
    
    oldMousePos = {mouseX, mouseY, 0.0};

    vec4 stick = getSticks();
    XMFLOAT2 Linput;
    XMFLOAT2 Rinput;
    {
        XMStoreFloat2( &Linput, XMVector2Normalize( XMLoadFloat2(&XMFLOAT2(stick.x, stick.y))) * CURSORSPEED);
        CursorPos.x   += Linput.x;
        CursorPos.y   += Linput.y;
    }

    if((fabs(stick.z) > 0.8f || fabs(stick.w) > 0.8f))
    {
        if(start)
        {
            float mag;
            XMStoreFloat( &mag, XMVector2Length(XMLoadFloat2(&XMFLOAT2(stick.z, stick.w))));
            XMStoreFloat2( &Rinput, XMVector2Normalize( XMLoadFloat2(&XMFLOAT2(stick.z, stick.w))) * 200.0f * mag    );
            slicePosEnd.x  = CursorPos.x + Rinput.x;
            slicePosEnd.y  = CursorPos.y + Rinput.y;
            if(AttCounter < 1)
            {
                slicePosStart  = CursorPos;
                SetBulletFreeRotate(slicePosStart, slicePosEnd);
                AttCounter = ATTACK_COOLDOWN;
            }
            start = FALSE;
        }
    }
    else
    {
        float mag;
        XMStoreFloat( &mag, XMVector2Length(XMLoadFloat2(&XMFLOAT2(stick.z, stick.w))));
        XMStoreFloat2( &Rinput, XMVector2Normalize( XMLoadFloat2(&XMFLOAT2(stick.z, stick.w))) * 200.0f * mag    );
        slicePosEnd.x  = CursorPos.x + Rinput.x;
        slicePosEnd.y  = CursorPos.y + Rinput.y;
        if ((fabs(stick.z) > 0.4f || fabs(stick.w) > 0.4f))
            start = TRUE;
    }



    if (GetKeyboardPress(DIK_LEFT))
    {
        CursorPos.x -= CURSORSPEED;
    }
    if (GetKeyboardPress(DIK_RIGHT))
    {
        CursorPos.x += CURSORSPEED;
    }
    if (GetKeyboardPress(DIK_UP))
    {
        CursorPos.y -= CURSORSPEED;
    }
    if (GetKeyboardPress(DIK_DOWN))
    {
        CursorPos.y += CURSORSPEED;
    }

    if (CursorPos.y > SCREEN_HEIGHT)    // 自分の大きさを考慮して画面外か判定している
    {
        CursorPos.y = SCREEN_HEIGHT;
    }
    if (CursorPos.y < 0.0)              // 自分の大きさを考慮して画面外か判定している
    {
        CursorPos.y = 0.0;
    }
    if (CursorPos.x < 0.0)              // 自分の大きさを考慮して画面外か判定している
    {
        CursorPos.x = 0.0;
    }
    if (CursorPos.x > SCREEN_WIDTH)              // 自分の大きさを考慮して画面外か判定している
    {
        CursorPos.x = SCREEN_WIDTH;
    }




    if (GetKeyboardTrigger(DIK_SPACE))//@
    {
    	if(AttCounter < 1)
    	{
    		SetBullet(CursorPos);
    		AttCounter = ATTACK_COOLDOWN;
    	}
    }

	if (IsMouseLeftTriggered())
	{
	    slicePosStart = CursorPos;
        start = TRUE;
	}

	if(IsMouseLeftReleased())
	{
        if (AttCounter < 1)
        {
            slicePosEnd = CursorPos;
            if(start)
            {
                SetBulletFreeRotate(slicePosStart, slicePosEnd);
                AttCounter = ATTACK_COOLDOWN;
            }
        }
        start = FALSE;
	}

    if (IsMouseRightTriggered() && IsMouseLeftHeld())
    {
        start = FALSE;
    }
#ifdef _DEBUG	// デバッグ情報を表示する


#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
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

    // テクスチャ設定
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

    //プレイヤーの位置やテクスチャー座標を反映
    float px = CursorPos.x;	// プレイヤーの表示位置X
    float py = CursorPos.y;	// プレイヤーの表示位置Y
    float pw = CURSORSIZE;		// プレイヤーの表示幅
    float ph = CURSORSIZE;		// プレイヤーの表示高さ

    // アニメーション用
    //float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
    //float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
    //float tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
    //float ty = (float)(g_Player[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

    float tw = 1.0f;	// テクスチャの幅
    float th = 1.0f;							// テクスチャの高さ
    float tx = tw;	// テクスチャの左上X座標
    float ty = 0.0f;							// テクスチャの左上Y座標

    // １枚のポリゴンの頂点とテクスチャ座標を設定
    SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
    	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // ポリゴン描画
    GetDeviceContext()->Draw(4, 0);

    if(start)
    {
        float px = slicePosEnd.x;
        float py = slicePosEnd.y;
        SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
    	XMFLOAT4(1.0f, 1.0f, 1.0f, 0.7f));
        
        GetDeviceContext()->Draw(4, 0);
    }

    
}


//=============================================================================
// Player構造体の先頭アドレスを取得
//=============================================================================}




