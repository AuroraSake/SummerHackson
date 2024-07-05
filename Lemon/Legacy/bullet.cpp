//=============================================================================
//
// バレット処理 [bullet.cpp]
// Author : 
//
//=============================================================================
#include "bullet.h"
#include "projectile.h"
#include "collision.h"
#include "score.h"
#include "effect.h"
#include "sound.h"
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SLASH_WIDTH				    (150)	// キャラサイズ
#define SLASH_HEIGHT				(75)	// 
#define BLADE_WIDTH				    (30 * 0.5f)	// キャラサイズ
#define BLADE_HEIGHT				(1000 * 0.5f)	// 
#define TEXTURE_MAX					(2)		// テクスチャの数

#define BLADE_POS_Y					400
#define BLADE_POS_X					480

#define BULLET_MAX		            (15)		// バレットのMax数
#define SLASH_MAX		            (15)		// バレットのMax数
#define BLADE_SPAN		            (10)
#define SLASH_SPAN		            (80)

#define TEXTURE_PATTERN_DIVIDE_X	(1)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(4)		// アニメーションの切り替わるWait値


#define RAYCAST_PERCISION           (5.0f)
#define SCREEN_BOUNDARY             ({0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT})

#define SLASH_OFFSET                (250.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[] = {
	"data/TEXTURE/bullet00.png",
	"data/TEXTURE/bullet01.png",
};

static BOOL		g_Load = FALSE;			// 初期化を行ったかのフラグ
static BULLET	g_Bullet[BULLET_MAX];	// バレット構造体

PROJECTILE *projectile;

static SLASH	g_Slash[SLASH_MAX];

static float g_Bullet_T[BULLET_MAX];
static XMFLOAT3 SpawnLoc[BULLET_MAX];

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBullet(void)
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
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_Bullet[i].use   = FALSE;			// 未使用（発射されていない弾）
		g_Bullet[i].w     = BLADE_WIDTH;
		g_Bullet[i].h     = BLADE_HEIGHT;
		g_Bullet[i].pos   = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].iPos  = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].rot   = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].texNo = 0;
	}

	//voidslash構造体初期化
	for (int i = 0; i < SLASH_MAX; i++)
	{
		g_Slash[i].use    = FALSE;									// true:使っている  false:未使用
		g_Slash[i].w      = SLASH_WIDTH;
		g_Slash[i].h      = SLASH_HEIGHT;							// 幅と高さ
		g_Slash[i].pos    = XMFLOAT3(300, 300.0f, 0.0f);;				// バレットの座標
		g_Slash[i].span   = -1;
	}

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
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
void UpdateBullet(void)
{
	int bulletCount = 0;				// 処理したバレットの数
	projectile = GetProjectile();

	BOOL isHit = FALSE;

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == TRUE)	// このバレットが使われている？
		{								// Yes

			g_Bullet[i].span--;

			// アニメーション  
			g_Bullet[i].countAnim++;
			if ((g_Bullet[i].countAnim % ANIM_WAIT) == 0)
			{
				// パターンの切り替え
				g_Bullet[i].patternAnim = (g_Bullet[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}
            float dx = BULLET_SPEED * cosf(g_Bullet[i].rot.z);
            float dy = BULLET_SPEED * sinf(g_Bullet[i].rot.z);
			
            g_Bullet[i].pos.x += dx;
			g_Bullet[i].pos.y += dy;

			// 当たり判定処理
			
			//for(int j = 0; j < PROJECTILE_MAX; j++)
			//{
			//	if(projectile[j].use)
			//		if(CollisionBB(XMFLOAT3(BLADE_POS_X, g_Bullet[i].pos.y, 0), 960, g_Bullet[i].w * 2, projectile[j].pos, projectile[j].w * 0.5f, projectile[j].h * 0.5f))
			//		    isHit = applyHit(projectile[j].pos, j);
			//}

            isHit = hitScan(g_Bullet[i].rot.z, g_Bullet[i].iPos);

			//Check_IfHitEnemy(i);

			if(g_Bullet[i].span < 1)
				g_Bullet[i].use = FALSE;

			bulletCount++;
		}
	}

	for (int i = 0; i < SLASH_MAX; i++)
	{
		if (g_Slash[i].use == TRUE)
		{
			for (int j = 0; j < PROJECTILE_MAX; j++)
			{
				if (projectile[j].use)
					if (CollisionBB(XMFLOAT3(g_Slash[i].pos.x, g_Slash[i].pos.y, 0), 400, 400, projectile[j].pos, projectile[j].w * 0.5f, projectile[j].h * 0.5f))
					{
                        //Slashは爆弾に当たらない
						if (projectile[j].texNo != FRUIT_HABANERO)
						{
                            float slashRot = rand() % 21 * 0.1f * PI;
                            XMFLOAT3 pos;
                            pos.x = - projectile[j].w * 0.35f + rand() % ((int)projectile[j].w + 1) * 0.7f;
                            pos.y = - projectile[j].h * 0.35f + rand() % ((int)projectile[j].h + 1) * 0.7f;
                            pos.z =   0.0f;
                            { 
                                float Sr = sinf(projectile[j].rot.z);
                                float Cr = cosf(projectile[j].rot.z);
                                float xOld = pos.x;
                                float yOld = pos.y;
                                pos.x = xOld * Cr - yOld * Sr;
                                pos.y = xOld * Sr + yOld * Cr;
                            }
							SetEffect(projectile[j].pos);
							CutProjectile(j, slashRot, pos);
							//projectile[j].use     = FALSE;
							//AddScore(100);
							isHit = TRUE;
						}
					}
			}

			g_Slash[i].span--;
			if(g_Slash[i].span < 1)
			{
				g_Slash[i].use = FALSE;
				continue;
			}

			if(g_Slash[i].span % 20);
			else
			{
                float Lspan = g_Slash[i].span * 0.05;
				switch((int)Lspan)
				{
				case 0:
					break;
				case 1:
					g_Slash[i].pos.x -= 200;
					break;
				case 2:
					g_Slash[i].pos.y += 200;
					break;
				case 3:
					g_Slash[i].pos.x += 200;
					break;
				}
			}
		}
	}

	if (isHit)
	{
		PlaySound(SOUND_LABEL_SE_cut);
	}
	else
	{
		PlaySound(SOUND_LABEL_SE_whiff);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
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

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == TRUE)		// このバレットが使われている？
		{									// Yes
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Bullet[i].texNo]);

			//バレットの位置やテクスチャー座標を反映
			float px = g_Bullet[i].pos.x;	// バレットの表示位置X
			float py = g_Bullet[i].pos.y;	// バレットの表示位置Y
			float pw = g_Bullet[i].w;		// バレットの表示幅
			float ph = g_Bullet[i].h;		// バレットの表示高さ

			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
			float tx = (float)(g_Bullet[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
			float ty = (float)(g_Bullet[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColorRotation(g_VertexBuffer, 
				px, py, pw, ph, 
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_Bullet[i].rot.z + PI * 0.5f);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	for (int i = 0; i < SLASH_MAX; i++)
	{
		if (g_Slash[i].use == TRUE)		// このバレットが使われている？
		{
			// 加減算のテスト
			//SetBlendState(BLEND_MODE_ADD);		// 加算合成
			//SetBlendState(BLEND_MODE_SUBTRACT);	// 減算合成
			/*for (int i = 0; i < 4; i++)
			{*/
				// テクスチャ設定


				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				float sx = (float)(rand() % 300);
				float sy = (float)(rand() % 300);

				float rot = rand() % 16 * PI * 0.125f; 

				SetSpriteColorRotation(g_VertexBuffer, (g_Slash[i].pos.x + sx), (g_Slash[i].pos.y + sy), g_Slash[i].w, g_Slash[i].h, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 0.6f), rot);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			//}
			//SetBlendState(BLEND_MODE_ALPHABLEND);	// 半透明処理を元に戻す
		}
	}
}


//=============================================================================
// バレット構造体の先頭アドレスを取得
//=============================================================================
BULLET *GetBullet(void)
{
	return &g_Bullet[0];
}


//=============================================================================
// バレットの発射設定
//=============================================================================
void SetBullet(XMFLOAT3 pos)
{

	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == FALSE)		// 未使用状態のバレットを見つける
		{
			g_Bullet[i].use  = TRUE;			// 使用状態へ変更する

            g_Bullet[i].pos   = XMFLOAT3(0.0, pos.y, 0.0);      // 中心座標をセット
            g_Bullet[i].iPos  = XMFLOAT3(1.0, pos.y, 0.0);      // 中心座標をセット
            g_Bullet[i].rot.z = 0.0;
            g_Bullet[i].span  = BLADE_SPAN;

			return;							// 1発セットしたので終了する
		}
	}
}

void SetBulletFreeRotate(XMFLOAT3 posStart,XMFLOAT3 posEnd)
{
    XMVECTOR end  = XMLoadFloat3(&posEnd);
    XMVECTOR vector = XMLoadFloat3(&posStart) - end;
    float    rot    = atan2f(vector.m128_f32[1], vector.m128_f32[0]);// +3.14f;
    

    // もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
    for (int i = 0; i < BULLET_MAX; i++)
    {
        if (g_Bullet[i].use == FALSE)		// 未使用状態のバレットを見つける
        {
            XMFLOAT4 screenBound = {0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT};
            BOOL inBound = isInBound(posStart, screenBound);
            XMFLOAT3 newPos = posStart;
            while(inBound)
            {
                newPos  = rayCast(rot, newPos);
                inBound = isInBound(newPos, screenBound);
            }
            XMFLOAT3 centerPos = rayCast(rot + PI, newPos);

            g_Bullet[i].use   = TRUE;           // 使用状態へ変更する
            g_Bullet[i].pos   = newPos;      // 中心座標をセット
            g_Bullet[i].iPos  = centerPos;      // 中心座標をセット
            g_Bullet[i].rot.z = rot + PI;
            g_Bullet[i].span  = BLADE_SPAN;

            return;							// 1発セットしたので終了する
        }
    }
}

void SetSlash(XMFLOAT3 pos)
{
	float X = pos.x;
	float Y = pos.y;

	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Slash[i].use == FALSE)		// 未使用状態のバレットを見つける
		{	  
			g_Slash[i].use  = TRUE;			// 使用状態へ変更する
			g_Slash[i].pos  = XMFLOAT3(X - SLASH_OFFSET, Y - SLASH_OFFSET, 0);			// 座標をセット
			g_Slash[i].span = SLASH_SPAN;

			return;							// 1発セットしたので終了する
		}
	}
}

BOOL isInBound(XMFLOAT3 pos, XMFLOAT4 bound)
{
    //use a float4 to store the 4 boundary of the bound
    return ((pos.x > bound.x) && (pos.x < bound.y) && (pos.y > bound.z) && (pos.y < bound.w));
}


XMFLOAT3 rayCast(float rot, XMFLOAT3 anker)
{
    anker.x += RAYCAST_PERCISION * cosf(rot);
    anker.y += RAYCAST_PERCISION * sinf(rot);
    return anker;
}

XMFLOAT3 rayCastH(float rot, XMFLOAT3 anker)
{
    anker.x += 1.0f * cosf(rot);
    anker.y += 1.0f * sinf(rot);
    return anker;
}

BOOL hitScan(float rot, XMFLOAT3 anker)
{
    XMFLOAT4 bound = {0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT};
    BOOL inBound = isInBound(anker, bound);
    BOOL anyHit = FALSE;
    while (inBound)
    {
        anker = rayCast(rot, anker);
        inBound = isInBound(anker, bound);
        for (int j = 0; j < PROJECTILE_MAX; j++)
        {
            if (projectile[j].use)
            {
                //rayCastの判定が細い代わりに、敵範囲を1.1倍にして調整する
                float offsetH = projectile[j].h * 0.35f;
                float offsetW = projectile[j].w * 0.35f;
                //XMFLOAT4 enemyHitbox = { projectile[j].pos.x - offsetW, projectile[j].pos.x + offsetW,
                //                         projectile[j].pos.y - offsetH, projectile[j].pos.y + offsetH};
                XMFLOAT4 enemyHitbox = { - offsetW, offsetW,
                                         - offsetH, offsetH};
                 
                XMFLOAT3 relativePos = {anker.x - projectile[j].pos.x, anker.y - projectile[j].pos.y , 0.0};
                { 
                    float Sr = sinf(-projectile[j].rot.z);
                    float Cr = cosf(-projectile[j].rot.z);
                    float xOld = relativePos.x;
                    float yOld = relativePos.y;
                    relativePos.x = xOld * Cr - yOld * Sr;
                    relativePos.y = xOld * Sr + yOld * Cr;
                }
                if(isInBound(relativePos, enemyHitbox))
                {
                    SetEffect(anker);
                    applyHit(relativePos, j, rot);
                    //applyHit(anker, j, rot);
                    anyHit = TRUE;
                }
            }
        }
    }
    return anyHit;
}

BOOL applyHit(XMFLOAT3 pos, int index, float rot)
{
    CutProjectile(index, rot, pos);
    //projectile[j].use     = FALSE;
    //AddScore(100);
    return TRUE;
}

XMVECTOR Bezier(XMVECTOR V0, XMVECTOR V1, float t)
{
	return V0 * t + t * (V0 * (1.0f - t) + V1 * t);
}
