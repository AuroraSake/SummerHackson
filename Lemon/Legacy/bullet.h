//=============================================================================
//
// バレット処理 [bullet.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PI							(3.14159265358979f)
#define BULLET_SPEED	            (100.0f)		// バレットの移動スピード


// バレット構造体
struct BULLET
{
	BOOL				use;				// true:使っている  false:未使用
	float				w, h;				// 幅と高さ
	XMFLOAT3			pos;				// バレットの座標
	XMFLOAT3			iPos;		    	// バレット発生したの座標
	XMFLOAT3			rot;				// バレットの回転量
	int					span;				//存在時間
	int					texNo;				// 何番目のテクスチャーを使用するのか
	int					countAnim;			// アニメーションカウント
	int					patternAnim;		// アニメーションパターンナンバー
};

//XMFLOAT3			move;				// バレットの移動量


struct SLASH
{
	BOOL				use;				// true:使っている  false:未使用
	float				w, h;				// 幅と高さ
	XMFLOAT3			pos;				// バレットの座標
	int					span;				// バレットの座標
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

BULLET *GetBullet(void);
void SetBullet(XMFLOAT3 pos);
void SetBulletFreeRotate(XMFLOAT3 posStart, XMFLOAT3 posEnd);
void SetSlash(XMFLOAT3 pos);


XMVECTOR Bezier(XMVECTOR V0, XMVECTOR V1, float t);

BOOL isInBound(XMFLOAT3 pos, XMFLOAT4 bound);
XMFLOAT3 rayCast(float rot, XMFLOAT3 anker);
XMFLOAT3 rayCastH(float rot, XMFLOAT3 anker);
BOOL hitScan(float rot, XMFLOAT3 anker);
BOOL applyHit(XMFLOAT3 pos, int index, float rot);