//=============================================================================
//
// Projectile���� [projectile.cpp]
// Author : 
//
//=============================================================================
#include "projectile.h"
#include "collision.h"
#include "score.h"
#include "timer.h"
#include "bullet.h"


#pragma once
#include <DirectXMath.h>
#include <math.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(100)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(100)	// 
#define TEXTURE_MAX					(6)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(4)		// �A�j���[�V�����̐؂�ւ��Wait�l

#define SPEED_MAX_Y                 (12.0f)
#define SPEED_RANGE_Y               34 * 0.01f + 67 * 0.01f
 
#define PASSED_MOMENTEM             (10)
#define PASSED_ROTATION             (PI * 0.2f)

#define PASSED_RANGE                26 * 0.01f + 75 * 0.01f
#define DECAY_LIFE                  50

#define PRECISION_FLOAT_EQUALITY    0.0001f
#define LARGE_NUMBER                 99999.9f

#define HARD    true

#ifdef HARD
#define INTERVAL_MIN        100
#define INTERVAL_RANGE      300
#else
#define INTERVAL_MIN        500
#define INTERVAL_RANGE      1000
#endif

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[] = {
	"data/TEXTURE/fruit_00.png",
	"data/TEXTURE/fruit_01.png",
	"data/TEXTURE/fruit_02.png",
	"data/TEXTURE/fruit_03.png",
	"data/TEXTURE/fruit_04.png",
	"data/TEXTURE/fruit_06.png",//ult

};

static BOOL		g_Load = FALSE;			// ���������s�������̃t���O
static PROJECTILE	g_Projectile[PROJECTILE_MAX];	// �o���b�g�\����


static CUT	g_Cut[ 2 * PROJECTILE_MAX];	// �o���b�g�\����



float g_timerStart = 0;
float g_timerInterval = 0;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitProjectile(void)
{
	ID3D11Device* pDevice = GetDevice();

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
	bd.ByteWidth = sizeof(VERTEX_3D) * 5;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �o���b�g�\���̂̏�����
	for (int i = 0; i < PROJECTILE_MAX; i++)
	{
		g_Projectile[i].use = FALSE;			// ���g�p�i���˂���Ă��Ȃ��e�j
		g_Projectile[i].w = TEXTURE_WIDTH;
		g_Projectile[i].h = TEXTURE_HEIGHT;
		g_Projectile[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Projectile[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Projectile[i].texNo = 0;

		g_Projectile[i].rotRate = 0;

		g_Projectile[i].move = XMFLOAT3(0.0f, 0.0, 0.0f);	// �ړ��ʂ�������
	}


	// CUT�̏�����
	for (int i = 0; i < PROJECTILE_MAX; i++)
	{
		g_Cut[i].prj.use = FALSE;			// ���g�p�i���˂���Ă��Ȃ��e�j
		g_Cut[i].prj.w = TEXTURE_WIDTH;
		g_Cut[i].prj.h = TEXTURE_HEIGHT;
		g_Cut[i].prj.pos = XMFLOAT3(300, 300.0f, 0.0f);
		

		g_Cut[i].prj.move = XMFLOAT3(0.0f, 0.0, 0.0f);	// �ړ��ʂ�������
	}

	ResetInterval();

	g_Load = TRUE;
	return S_OK;
}

void ResetInterval(void)
{
	g_timerStart = (float)timeGetTime();
	g_timerInterval = (float)(rand() % INTERVAL_RANGE) + (float)INTERVAL_MIN;
}


//=============================================================================
// �I������
//=============================================================================
void UninitProjectile(void)
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
void UpdateProjectile(void)
{
	int projectileCount = 0;				// ���������o���b�g�̐�

	for (int i = 0; i < PROJECTILE_MAX; i++)
	{
		if (g_Projectile[i].use == TRUE)	// ���̃o���b�g���g���Ă���H
		{								// Yes

			// �o���b�g�̈ړ�����
			XMVECTOR pos = XMLoadFloat3(&g_Projectile[i].pos);
			XMVECTOR move = XMLoadFloat3(&g_Projectile[i].move);

			//�ړ�����
			pos += move;

			{	//�d��
					g_Projectile[i].move.y += GRAVITY;
			}
            g_Projectile[i].rot.z += g_Projectile[i].rotRate;

			XMStoreFloat3(&g_Projectile[i].pos, pos);

			// ��ʉ�����o����
			//��ʊO�������΂���悤�Ɂ@�]���ȕ��̌��ԁ��L���G���A����������
			if (g_Projectile[i].pos.y > (SCREEN_HEIGHT + g_Projectile[i].h * 0.5f) + KILL_Y)	// �����̑傫�����l�����ĉ�ʊO�����肵�Ă���
			{
				g_Projectile[i].use = false;
			}

			//��ʂ̍��ɏo����
			if ((g_Projectile[i].pos.x < (0 - g_Projectile[i].w * 0.5f)) || (g_Projectile[i].pos.x > (SCREEN_WIDTH + g_Projectile[i].w * 0.5f)))	// �����̑傫�����l�����ĉ�ʊO�����肵�Ă���
			{
				g_Projectile[i].move.x = - g_Projectile[i].move.x;
			}
			projectileCount++;
		}
	}

	for (int i = 0; i < PROJECTILE_MAX; i++)
	{
		if (g_Cut[i].prj.use == TRUE)	// ���̃o���b�g���g���Ă���H
		{								// Yes
			
            if(g_Cut[i].span-- < 0)

            {
                g_Cut[i].prj.use = FALSE;
                //free(g_Cut[i].vertices);
                continue;
            }
            // �o���b�g�̈ړ�����
			XMVECTOR pos  = XMLoadFloat3(&g_Cut[i].prj.pos);
			XMVECTOR move = XMLoadFloat3(&g_Cut[i].prj.move);

			//�ړ�����
			pos += move;

            g_Cut[i].prj.rot.z += g_Cut[i].prj.rotRate;

            g_Cut[i].prj.move.y += GRAVITY;

			XMStoreFloat3(&g_Cut[i].prj.pos, pos);

			// ��ʉ�����o���� 
			if (g_Cut[i].prj.pos.y > (SCREEN_HEIGHT + g_Cut[i].prj.h / 2))	// �����̑傫�����l�����ĉ�ʊO�����肵�Ă���
			{
				g_Cut[i].prj.use = FALSE;
                //free(g_Cut[i].vertices);
			}			
			//��ʏォ��ł���
			if (g_Cut[i].prj.pos.y < (0 - g_Cut[i].prj.h / 2))	// �����̑傫�����l�����ĉ�ʊO�����肵�Ă���
			{
				g_Cut[i].prj.use = FALSE;
                //free(g_Cut[i].vertices);
			}
		}
	}



	if (timeGetTime() - g_timerStart >= g_timerInterval)
	{
		ResetInterval();
		RandomSpawn();
	}


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawProjectile(void)
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

	for (int i = 0; i < PROJECTILE_MAX; i++)
	{
		if (g_Projectile[i].use == TRUE)		// ���̃o���b�g���g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Projectile[i].texNo]);

			//�o���b�g�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Projectile[i].pos.x;	// �o���b�g�̕\���ʒuX
			float py = g_Projectile[i].pos.y;	// �o���b�g�̕\���ʒuY
			float pw = g_Projectile[i].w;		// �o���b�g�̕\����
			float ph = g_Projectile[i].h;		// �o���b�g�̕\������

			float tw = 1.0f;	// �e�N�X�`���̕�
			float th = 1.0f;	// �e�N�X�`���̍���
			float tx = tw;	// �e�N�X�`���̍���X���W
			float ty = th;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer,
				px, py, pw, ph,
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				g_Projectile[i].rot.z);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	//�J�b�g
	for (int i = 0; i < (2 * PROJECTILE_MAX); i++)
	{
		if (g_Cut[i].prj.use == TRUE)		// ���̃o���b�g���g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Cut[i].prj.texNo]);

			//�o���b�g�̈ʒu��e�N�X�`���[���W�𔽉f

            float alpha = g_Cut[i].span * 0.02f;
			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
            
            SetSpriteFreeVertex(g_VertexBuffer, g_Cut[i].prj.pos , g_Cut[i].prj.rot.z,
            	XMFLOAT4(1.0f, 1.0f, 1.0f, alpha), g_Cut[i].vertices, g_Cut[i].vertexNum);
		}
	}

}


//=============================================================================
// �o���b�g�\���̂̐擪�A�h���X���擾
//=============================================================================
PROJECTILE* GetProjectile(void)
{
	return &g_Projectile[0];
}


//=============================================================================
// �o���b�g�̔��ːݒ�
//=============================================================================
void SetProjectile(XMFLOAT3 pos, XMFLOAT3 move, float size, int texNo, float rotZ, int score)
{
	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int i = 0; i < PROJECTILE_MAX; i++)
	{
		if (g_Projectile[i].use == FALSE)		// ���g�p��Ԃ�Projectile��������
		{
			g_Projectile[i].use = TRUE;			// �g�p��Ԃ֕ύX����
			g_Projectile[i].pos = pos;			// ���W���Z�b�g
            if(g_Projectile[i].texNo == FRUIT_ULT)
                g_Projectile[i].rotRate = 0.0;
            else
			    g_Projectile[i].rotRate = rotZ;		// �p�x���Z�b�g
            float WtoHratio = 1.0f;
            switch(texNo)
            {
	            case FRUIT_BANANA:
	            	WtoHratio = 1.01f;
	            	break;
	            case FRUIT_PEAR:
	            	WtoHratio = 1.76f;
	            	break;
	            case FRUIT_PURPLE:
	            	WtoHratio = 1.01f;
	            	break;
	            case FRUIT_LEMON:
	            	WtoHratio = 1.64f;
	            	break;
	            case FRUIT_HABANERO:
	            	WtoHratio = 2.06f;
	            	break;
	            case FRUIT_ULT:
	            	WtoHratio = 1.618f;
	            	break;
            }
			g_Projectile[i].w = size;			// �X�P�[�����Z�b�g
			g_Projectile[i].h = size * WtoHratio;			// �X�P�[�����Z�b�g
			g_Projectile[i].texNo = texNo;		// �e�N�X�`�����Z�b�g

			g_Projectile[i].move = move;
			return;							// 1�Z�b�g�����̂ŏI������
		}
	}
}

//=============================================================================
// �o���b�g�̔��ːݒ�
//=============================================================================
void SetCut(PROJECTILE parent, float rot, BOOL upperPart)
{
	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int i = 0; i < PROJECTILE_MAX; i++)
	{
		if (g_Cut[i].prj.use == FALSE)		// ���g�p��Ԃ�Projectile��������
		{
			g_Cut[i].prj.use  = TRUE;			// �g�p��Ԃ֕ύX����
			g_Cut[i].prj      = parent;		// �g�p��Ԃ֕ύX����
			g_Cut[i].span     = DECAY_LIFE;		// ���ݎ���


            float speedModX = PASSED_MOMENTEM * cosf(rot) * (rand() % PASSED_RANGE);
            float speedModY = PASSED_MOMENTEM * sinf(rot) * (rand() % PASSED_RANGE);

			g_Cut[i].prj.move = XMFLOAT3(parent.move.x + speedModX , parent.move.y + speedModY , 0);
			g_Cut[i].prj.rot.z = parent.rot.z;

            float rotRateBase = PASSED_ROTATION;
            if(!upperPart)
            {
                g_Cut[i].prj.rotRate *= - 1.0f;
                rotRateBase          *= - 1.0f;
            }
			g_Cut[i].prj.rotRate = 2.4f * parent.rotRate * (rand() % PASSED_RANGE);
			g_Cut[i].prj.rotRate = rotRateBase + g_Cut[i].prj.rotRate;

			return;
		}
	}
}

void SetCutFreeShape(PROJECTILE parent, float rot, XMFLOAT3 pos)
{
    float width = parent.w * 0.5f;
    float hight = parent.h * 0.5f;
    
    XMFLOAT4 intersects   [2];
    int      resultParent [4] = { 0, 0, 0, 0};// 0:both parts. 1: upperPart, -1: lowerPart
    int      iniIndex         = 0;

    XMFLOAT4 spritevertices[4] = 
    {   //for easier computation, use clockwise order for the vertices rather than Z order
        //�v�Z���₷�����߁A���v���̒��_�ԍ����g��
        {-width, -hight, 0.0f, 0.0f},   //LU
        { width, -hight, 1.0f, 0.0f},   //RU
        { width,  hight, 1.0f, 1.0f},   //RD
        {-width,  hight, 0.0f, 1.0f},   //LD //���v�� ----clockwise
    };

    float rot_R = regulate(rot - parent.rot.z);
    {   
        for(int i = 0; i < 2; i++)
        {
            XMFLOAT3 temp0 = pos;
            XMFLOAT3 temp1 = temp0;

            float rot_rayCast = rot - parent.rot.z + PI * (1 - i);

            //Sprite boundary
            //�؂�ꂽtexture�̘g
            XMFLOAT4 spriteBound = {- width,
                                      width,
                                    - hight,
                                      hight,};

            BOOL inBound = isInBound(temp0, spriteBound);
        
            while(inBound)
                //Use the first pos and rot parameter to rayCast the second insersect
            {   //rayCast���āA�͈͂ɓ����Ă����Ԙg�ɋ߂��_�����߂�
                temp1   = temp0;
                temp0   = rayCastH(rot_rayCast, temp0);
                inBound = isInBound(temp0, spriteBound);
            }
            //Store the two intersection points to be used as vertices later  ----uses relative coordnates inside the sprite
            //�ؒf���̓�̐ړ_���v�Z���ĕۑ�����                                ----�ړ_�̍��W��texture�̘g���̑��΍��W�ɕϊ�����
            intersects[i].x = temp1.x;
            intersects[i].y = temp1.y;
        }
    }
    XMFLOAT3 rayMid;        //�ؒf���̒��_ ---- the mid point of the two intersections
    rayMid.x = (intersects[0].x + intersects[1].x) * 0.5f;
    rayMid.y = (intersects[0].y + intersects[1].y) * 0.5f;

    for(int i = 0; i < 2; i++)
    {
        //calculates the UV for the intersections
        //�ړ_��UV���v�Z����
        intersects[i].z = (intersects[i].x + parent.w * 0.5f) / parent.w;       //U
        intersects[i].w = (intersects[i].y + parent.h * 0.5f) / parent.h;       //V
    }


    int upperIndex = 0;
    int lowerIndex = 0;
    for(int i = 0; i < 4; i++)
    {
        float raw_C = atan2f(spritevertices[i].y - intersects[0].y, spritevertices[i].x - intersects[0].x);
        float rot_C = regulateA(raw_C - rot_R);
        if (rot_C > 0.0f)
        {
            resultParent[i] =   1;
        }
        else if (rot_C < 0.0f)
        {
            resultParent[i] = - 1;
        }
    }
    for(int i = 0; i < 4; i++)
    {
        if(resultParent[i] <= 0 && resultParent[i + 1] > 0 )
            iniIndex = i;
    }

    //setting up the two generated sprite by the division
    //��̐ؒf���ꂽsprite��ݒ肷��
    for(int j = 0; j < 2; j++)
    {
        int  partOrien = 1;
        BOOL lowerPart = j;

        if  (lowerPart)
            partOrien = -1;
        // �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
        for (int k = 0; k < PROJECTILE_MAX; k++)
        {
            if (g_Cut[k].prj.use == FALSE)		// ���g�p��Ԃ�Projectile��������
            {
                XMFLOAT2 center;
                {
                    //boundBox centriod
                    //���^�ߎ����S�_
                    XMFLOAT4 extremums = { LARGE_NUMBER,  //minX 
                                           LARGE_NUMBER,  //minY 
                                         - LARGE_NUMBER,  //maxX 
                                         - LARGE_NUMBER,};//maxY 
                    for(int i = 0; i < 4; i++)
                    {
                        if(resultParent[i] * partOrien > 0 || partOrien == 0)
                        {
                            float x = spritevertices[i].x;
                            float y = spritevertices[i].y;
                            extremums = checkExtremum( XMFLOAT2(x, y) , extremums);
                        }
                    }
                    for(int i = 0; i < 2; i++)
                    {
                        float x = intersects[i].x;
                        float y = intersects[i].y;
                        extremums = checkExtremum( XMFLOAT2(x, y) , extremums);
                    }
                    center.x = (extremums.x + extremums.z) * 0.5f;
                    center.y = (extremums.y + extremums.w) * 0.5f;
                }

                int indexer = 0;
                int vertexNum = 0;
                for(int i = 0; i < 4; i++)
                {
                    int index = (iniIndex + i) % 4;
                    if(resultParent[index] * partOrien > 0)
                    {
                        g_Cut[k].vertices[vertexNum] = spritevertices[index];
                        g_Cut[k].vertices[vertexNum].x -= center.x;
                        g_Cut[k].vertices[vertexNum].y -= center.y;
                        vertexNum++;
                        if(vertexNum > 1)
                        {
                            if(!lowerPart)
                                g_Cut[k].vertices[vertexNum] = intersects[1 - indexer];
                            else
                                g_Cut[k].vertices[vertexNum] = intersects[indexer];
                            g_Cut[k].vertices[vertexNum].x -= center.x;
                            g_Cut[k].vertices[vertexNum].y -= center.y;
                            vertexNum++;
                            indexer++;
                        }
                    }
                }
                if(indexer < 1)
                {
                    for(; indexer < 2; indexer++)
                    {
                        if(lowerPart)
                            g_Cut[k].vertices[vertexNum] = intersects[1 - indexer];
                        else
                            g_Cut[k].vertices[vertexNum] = intersects[indexer];
                        g_Cut[k].vertices[vertexNum].x -= center.x;
                        g_Cut[k].vertices[vertexNum].y -= center.y;
                        vertexNum++;
                    }
                }
                else if(indexer < 2)
                {
                    if(!lowerPart)
                        g_Cut[k].vertices[vertexNum] = intersects[1 - indexer];
                    else
                        g_Cut[k].vertices[vertexNum] = intersects[indexer];
                    g_Cut[k].vertices[vertexNum].x -= center.x;
                    g_Cut[k].vertices[vertexNum].y -= center.y;
                    vertexNum++;
                    indexer++;
                }

                g_Cut[k].prj.use = TRUE;		    // �g�p��Ԃ֕ύX����
                g_Cut[k].prj     = parent;		    // �g�p��Ԃ֕ύX����
                g_Cut[k].span    = DECAY_LIFE;		// ���ݎ���

                float momentemRot = rot + partOrien * PASSED_ROTATION;
                float rotRateMod  = partOrien * PASSED_ROTATION * 0.25f;

                float speedModX = PASSED_MOMENTEM * cosf(momentemRot) * (rand() % PASSED_RANGE);
                float speedModY = PASSED_MOMENTEM * sinf(momentemRot) * (rand() % PASSED_RANGE);
                
                g_Cut[k].prj.move     =  XMFLOAT3(parent.move.x + speedModX, parent.move.y + speedModY, 0);
                g_Cut[k].prj.pos      = {parent.pos.x + center.x, parent.pos.y + center.y, 0.0};
                g_Cut[k].prj.rotRate *= (rand() % PASSED_RANGE);
                g_Cut[k].prj.rotRate  =  rotRateMod + g_Cut[k].prj.rotRate;
                g_Cut[k].vertexNum    =  vertexNum;

                iniIndex = (iniIndex + (vertexNum - 2)) % 4;
                break;
            }
        }
    }
}

void RandomSpawn(void)
{
	//���S���W��ݒ�
	XMFLOAT3 pos = XMFLOAT3(SCREEN_CENTER_X, SCREEN_HEIGHT, 0);

	int randomX;
	{	//����X �����_��
		randomX = rand() % SCREEN_CENTER_X;

		if (rand() % 2)
		{	//���E�����_��
			randomX *= -1;
		}

		pos.x += randomX;
	}

	float randDirX = 0.0f;
    float randRot  = 0.0f;
	int RANDOM = 4;
	{	//���E�@��ԕ��������_��
		randDirX = (float)(rand() % RANDOM);
        randRot  = (RANDOM + 1 + rand() % RANDOM) * 0.00125f * PI;

		if (rand() % 2)
		{	//���E�����_��
			randDirX *= -1;
		}
	}
	
	float randDirY = SPEED_MAX_Y * (rand() % SPEED_RANGE_Y);
	
	float size = 0;

	int fruit = 0;// FRUIT_MAX;
	int fruitrand = rand() % 100 + 1;

#ifdef TEST

    fruit = FRUIT_PEAR;
	size = 60;
	
#else
	if (fruitrand <= 10)
	{
		fruit = FRUIT_ULT;
	}
	else if (fruitrand <= 30)
	{
		fruit = FRUIT_HABANERO;
	}
	else if (fruitrand <= 50)
	{
		fruit = FRUIT_BANANA;
	}
	else if (fruitrand <= 70)
	{
		fruit = FRUIT_PEAR;
	}
	else if (fruitrand <= 90)
	{
		fruit = FRUIT_PURPLE;
	}
	else 
	{
		fruit = FRUIT_LEMON;
	}
#endif

	switch (fruit)
	{
	case FRUIT_BANANA:
		size = 150;
		break;
	case FRUIT_PEAR:
		size = 80;
		break;
	case FRUIT_PURPLE:
		size = 60;
		break;
	case FRUIT_LEMON:
		size = 50;
		break;
	case FRUIT_HABANERO:
		size = 100;
		break;
	case FRUIT_ULT:
		size = 150;
		break;
	}

	//Projectile����
	//SetProjectile(pos, XMFLOAT3(randDirX, -randDirY, 0), size, fruit, 0.0f, 100);
	SetProjectile(pos, XMFLOAT3(randDirX, -randDirY, 0), size, fruit, randRot, 100);
}

void CutProjectile(int projectileID, float rot, XMFLOAT3 pos)
{

	//score

	switch (g_Projectile[projectileID].texNo)
	{
	case FRUIT_BANANA:
		AddScore(50);
		break;	
	case FRUIT_PEAR:
		AddScore(50);
		break;
	case FRUIT_PURPLE:
		AddScore(150);
		break;
	case FRUIT_LEMON:
		AddScore(200);
		break;
	case FRUIT_HABANERO:
		PassTime(5);
		//AddScore(0);
		break;
	case FRUIT_ULT:
		//Ultimate();
        PassTime(-3);
		SetSlash(g_Projectile[projectileID].pos);
		//AddScore(0);
		break;
	}

	//SetCut(g_Projectile[projectileID], rot, TRUE );
	//SetCut(g_Projectile[projectileID], rot, FALSE);
    SetCutFreeShape(g_Projectile[projectileID], rot, pos);
	g_Projectile[projectileID].use = FALSE;
}


float regulate(float rot)
{
    int sign =   1;
    if( rot < 0)
        sign = - 1;
    //regulate the angle to a range of 0 to 2PI for easier comparison
    //��r���₷���悤�Aangle�� 0 ���� 2PI �͈̔͂ɐ��K������
    while ( rot * sign > PI)
    {
        rot += (2 * PI) * (-sign);
    }
    if(rot < 0)
            rot += 2 * PI;
    return rot;
}

float regulateA(float rot)
{
    int sign =   1;
    if( rot < 0)
        sign = - 1;
    //regulate the angle to a range of 0 to 2PI for easier comparison
    //��r���₷���悤�Aangle�� -PI ���� PI �͈̔͂ɐ��K������
    while ( rot * sign > PI)
    {
        rot += (2 * PI) * (-sign);
    }
    return rot;
}

float normalize(float rot, float rot_base)
{
    int sign =   1;
    if( rot_base < 0)
        sign = - 1;
    //angle��-PI����PI�͈̔͂ɐ��K������
    while ( rot_base * sign > PI)
    {
        rot_base += (2 * PI) * (-sign);
        sign = rot_base < 0 ? - 1 : 1;
    }
    rot = rot - rot_base;
    //angle��-PI/2����PI/2�͈̔͂ɐ��K������
    if (rot * sign > (PI * 0.5f))
        rot += PI * (-sign);
    return rot;
}

//XMFLOAT4 ApproxyVertexParam(PROJECTILE parent, int index)
//{
//    float U = 0.0f + (int)(index % 2);
//    float V = 0.0f + (int)(index / 2);
//
//    return XMFLOAT4(0.0, 0.0, U, V);
//}

XMFLOAT2 getCenterCoord (BOOL upperPart, XMFLOAT2 rayMid, XMFLOAT2 approxiMid)
{
    XMFLOAT2 center;
    center.x = (rayMid.x + approxiMid.x) * 0.5f;
    center.y = (rayMid.y + approxiMid.y) * 0.5f;
    return center;
}

XMFLOAT4 checkExtremum (XMFLOAT2 pos, XMFLOAT4 extremums)
{
    
    if(pos.x < extremums.x)
        extremums.x = pos.x;
    if(pos.y < extremums.y)
        extremums.y = pos.y;
    if(pos.x > extremums.z)
        extremums.z = pos.x;
    if(pos.y > extremums.w)
        extremums.w = pos.y;
    return extremums;
}