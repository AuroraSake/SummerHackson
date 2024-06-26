//=============================================================================
//
// Projectile処理 [projectile.h]
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
#define PROJECTILE_MAX		(100)			// ProjectileのMax数
#define PROJECTILE_SPEED	(10.0f)		// バレットの移動スピード

#define KILL_Y 50	//画面下からどれ位のｙが加算されたらProjectileが未使用にされるか

#define GRAVITY 0.163f			//重力定数 9.8/60frame
#define GRAVITY_ADDITIVE 0.1f	//重力加速度的な物


// Projectile構造体
struct PROJECTILE
{
	BOOL				use;				// true:使っている  false:未使用
	float				w, h;				// 幅と高さ
	XMFLOAT3			pos;				// バレットの座標
	XMFLOAT3			rot;				// バレットの回転量
	XMFLOAT3			move;				// バレットの移動量
	float				rotRate;			// 旋回スピード
	int					texNo;				// 何番目のテクスチャーを使用するのか

};

struct CUT
{
	PROJECTILE			prj;
	BOOL				useFreeShape;
    int                 span;
    //XMFLOAT4           *vertices;
    XMFLOAT4            vertices[6];
    int                 vertexNum;
};


enum
{
	FRUIT_PURPLE,
	FRUIT_LEMON,
	FRUIT_PEAR,
	FRUIT_BANANA,
	FRUIT_HABANERO,
	FRUIT_ULT,
	FRUIT_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitProjectile(void);
void UninitProjectile(void);
void UpdateProjectile(void);
void DrawProjectile(void);

PROJECTILE *GetProjectile(void);
void  SetProjectile(XMFLOAT3 pos, XMFLOAT3 move, float size, int texNo, float rotZ, int score);

void RandomSpawn(void);

void ResetInterval(void);
void CutProjectile(int projectileID, float rot, XMFLOAT3 pos);

void SetCut(PROJECTILE projectile, float rot, BOOL upperPart);

XMFLOAT4 ApproxyVertexParam(PROJECTILE parent, int index);
float normalize(float rot, float rot_base);
float regulate(float rot);
float regulateA(float rot);

void SetCutFreeShape(PROJECTILE parent, float rot, XMFLOAT3 pos);
void SetCutFreeRot(PROJECTILE parent, float rot);

XMFLOAT4 checkExtremum (XMFLOAT2 pos, XMFLOAT4 extremums);