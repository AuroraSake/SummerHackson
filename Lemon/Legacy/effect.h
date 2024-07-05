//=============================================================================
//
// バレット処理 [effect.h]
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
#define EFFECT_MAX		(15)		// バレットのMax数
#define EFFECT_SPEED	(200.0f)		// バレットの移動スピード
#define EFFECT_SPAN		(100)


// バレット構造体
struct EFFECT
{
	BOOL				use;				// true:使っている  false:未使用
	float				w, h;				// 幅と高さ
	XMFLOAT3			pos;				// バレットの座標
	XMFLOAT3			rot;				// バレットの回転量
	int					span;				//存在時間
	int					texNo;				// 何番目のテクスチャーを使用するのか
	int					countAnim;			// アニメーションカウント
	int					patternAnim;		// アニメーションパターンナンバー
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT   InitEffect(void);
void    UninitEffect(void);
void    UpdateEffect(void);
void      DrawEffect(void);

void SetEffect(XMFLOAT3 pos);
