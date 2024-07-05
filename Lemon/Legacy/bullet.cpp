//=============================================================================
//
// �o���b�g���� [bullet.cpp]
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
// �}�N����`
//*****************************************************************************
#define SLASH_WIDTH				    (150)	// �L�����T�C�Y
#define SLASH_HEIGHT				(75)	// 
#define BLADE_WIDTH				    (30 * 0.5f)	// �L�����T�C�Y
#define BLADE_HEIGHT				(1000 * 0.5f)	// 
#define TEXTURE_MAX					(2)		// �e�N�X�`���̐�

#define BLADE_POS_Y					400
#define BLADE_POS_X					480

#define BULLET_MAX		            (15)		// �o���b�g��Max��
#define SLASH_MAX		            (15)		// �o���b�g��Max��
#define BLADE_SPAN		            (10)
#define SLASH_SPAN		            (80)

#define TEXTURE_PATTERN_DIVIDE_X	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(4)		// �A�j���[�V�����̐؂�ւ��Wait�l


#define RAYCAST_PERCISION           (5.0f)
#define SCREEN_BOUNDARY             ({0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT})

#define SLASH_OFFSET                (250.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {
	"data/TEXTURE/bullet00.png",
	"data/TEXTURE/bullet01.png",
};

static BOOL		g_Load = FALSE;			// ���������s�������̃t���O
static BULLET	g_Bullet[BULLET_MAX];	// �o���b�g�\����

PROJECTILE *projectile;

static SLASH	g_Slash[SLASH_MAX];

static float g_Bullet_T[BULLET_MAX];
static XMFLOAT3 SpawnLoc[BULLET_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
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


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �o���b�g�\���̂̏�����
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_Bullet[i].use   = FALSE;			// ���g�p�i���˂���Ă��Ȃ��e�j
		g_Bullet[i].w     = BLADE_WIDTH;
		g_Bullet[i].h     = BLADE_HEIGHT;
		g_Bullet[i].pos   = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].iPos  = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].rot   = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].texNo = 0;
	}

	//voidslash�\���̏�����
	for (int i = 0; i < SLASH_MAX; i++)
	{
		g_Slash[i].use    = FALSE;									// true:�g���Ă���  false:���g�p
		g_Slash[i].w      = SLASH_WIDTH;
		g_Slash[i].h      = SLASH_HEIGHT;							// ���ƍ���
		g_Slash[i].pos    = XMFLOAT3(300, 300.0f, 0.0f);;				// �o���b�g�̍��W
		g_Slash[i].span   = -1;
	}

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	int bulletCount = 0;				// ���������o���b�g�̐�
	projectile = GetProjectile();

	BOOL isHit = FALSE;

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == TRUE)	// ���̃o���b�g���g���Ă���H
		{								// Yes

			g_Bullet[i].span--;

			// �A�j���[�V����  
			g_Bullet[i].countAnim++;
			if ((g_Bullet[i].countAnim % ANIM_WAIT) == 0)
			{
				// �p�^�[���̐؂�ւ�
				g_Bullet[i].patternAnim = (g_Bullet[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}
            float dx = BULLET_SPEED * cosf(g_Bullet[i].rot.z);
            float dy = BULLET_SPEED * sinf(g_Bullet[i].rot.z);
			
            g_Bullet[i].pos.x += dx;
			g_Bullet[i].pos.y += dy;

			// �����蔻�菈��
			
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
                        //Slash�͔��e�ɓ�����Ȃ�
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
// �`�揈��
//=============================================================================
void DrawBullet(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == TRUE)		// ���̃o���b�g���g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Bullet[i].texNo]);

			//�o���b�g�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Bullet[i].pos.x;	// �o���b�g�̕\���ʒuX
			float py = g_Bullet[i].pos.y;	// �o���b�g�̕\���ʒuY
			float pw = g_Bullet[i].w;		// �o���b�g�̕\����
			float ph = g_Bullet[i].h;		// �o���b�g�̕\������

			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			float tx = (float)(g_Bullet[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
			float ty = (float)(g_Bullet[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer, 
				px, py, pw, ph, 
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_Bullet[i].rot.z + PI * 0.5f);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	for (int i = 0; i < SLASH_MAX; i++)
	{
		if (g_Slash[i].use == TRUE)		// ���̃o���b�g���g���Ă���H
		{
			// �����Z�̃e�X�g
			//SetBlendState(BLEND_MODE_ADD);		// ���Z����
			//SetBlendState(BLEND_MODE_SUBTRACT);	// ���Z����
			/*for (int i = 0; i < 4; i++)
			{*/
				// �e�N�X�`���ݒ�


				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				float sx = (float)(rand() % 300);
				float sy = (float)(rand() % 300);

				float rot = rand() % 16 * PI * 0.125f; 

				SetSpriteColorRotation(g_VertexBuffer, (g_Slash[i].pos.x + sx), (g_Slash[i].pos.y + sy), g_Slash[i].w, g_Slash[i].h, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 0.6f), rot);

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
			//}
			//SetBlendState(BLEND_MODE_ALPHABLEND);	// ���������������ɖ߂�
		}
	}
}


//=============================================================================
// �o���b�g�\���̂̐擪�A�h���X���擾
//=============================================================================
BULLET *GetBullet(void)
{
	return &g_Bullet[0];
}


//=============================================================================
// �o���b�g�̔��ːݒ�
//=============================================================================
void SetBullet(XMFLOAT3 pos)
{

	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == FALSE)		// ���g�p��Ԃ̃o���b�g��������
		{
			g_Bullet[i].use  = TRUE;			// �g�p��Ԃ֕ύX����

            g_Bullet[i].pos   = XMFLOAT3(0.0, pos.y, 0.0);      // ���S���W���Z�b�g
            g_Bullet[i].iPos  = XMFLOAT3(1.0, pos.y, 0.0);      // ���S���W���Z�b�g
            g_Bullet[i].rot.z = 0.0;
            g_Bullet[i].span  = BLADE_SPAN;

			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}

void SetBulletFreeRotate(XMFLOAT3 posStart,XMFLOAT3 posEnd)
{
    XMVECTOR end  = XMLoadFloat3(&posEnd);
    XMVECTOR vector = XMLoadFloat3(&posStart) - end;
    float    rot    = atan2f(vector.m128_f32[1], vector.m128_f32[0]);// +3.14f;
    

    // �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
    for (int i = 0; i < BULLET_MAX; i++)
    {
        if (g_Bullet[i].use == FALSE)		// ���g�p��Ԃ̃o���b�g��������
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

            g_Bullet[i].use   = TRUE;           // �g�p��Ԃ֕ύX����
            g_Bullet[i].pos   = newPos;      // ���S���W���Z�b�g
            g_Bullet[i].iPos  = centerPos;      // ���S���W���Z�b�g
            g_Bullet[i].rot.z = rot + PI;
            g_Bullet[i].span  = BLADE_SPAN;

            return;							// 1���Z�b�g�����̂ŏI������
        }
    }
}

void SetSlash(XMFLOAT3 pos)
{
	float X = pos.x;
	float Y = pos.y;

	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Slash[i].use == FALSE)		// ���g�p��Ԃ̃o���b�g��������
		{	  
			g_Slash[i].use  = TRUE;			// �g�p��Ԃ֕ύX����
			g_Slash[i].pos  = XMFLOAT3(X - SLASH_OFFSET, Y - SLASH_OFFSET, 0);			// ���W���Z�b�g
			g_Slash[i].span = SLASH_SPAN;

			return;							// 1���Z�b�g�����̂ŏI������
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
                //rayCast�̔��肪�ׂ�����ɁA�G�͈͂�1.1�{�ɂ��Ē�������
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
