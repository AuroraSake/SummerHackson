//=============================================================================
//
// �o���b�g���� [Effect.cpp]
// Author : 
//
//=============================================================================
#include "effect.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(100)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(100)	// 
#define TEXTURE_MAX					(1)		// �e�N�X�`���̐�


#define TEXTURE_PATTERN_DIVIDE_X	(13)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(4)		// �A�j���[�V�����̐؂�ւ��Wait�l

#define PI							(3.14159265358979f)

//#define Effect_DV		(1)	// �o���b�g�̈ړ��X�s�[�h

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {
	"data/TEXTURE/smoke00.png",
};

static BOOL		g_Load = FALSE;			// ���������s�������̃t���O
static EFFECT	g_Effect[EFFECT_MAX];	// effect�\����

static float g_Effect_T[EFFECT_MAX];
static XMFLOAT3 SpawnLoc[EFFECT_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEffect(void)
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
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		g_Effect[i].use   = FALSE;			// ���g�p�i���˂���Ă��Ȃ��e�j
		g_Effect[i].w     = TEXTURE_WIDTH;
		g_Effect[i].h     = TEXTURE_HEIGHT;
		g_Effect[i].pos   = XMFLOAT3(300, 300.0f, 0.0f);
		g_Effect[i].rot   = XMFLOAT3(0.0f, 0.0f, PI/2.0f);
		g_Effect[i].texNo = 0;

		//g_Effect[i].countAnim = 0;
		//g_Effect[i].patternAnim = 0;

		//g_Effect[i].move = XMFLOAT3(0.0f, 0.0f, 0.0f);	// �ړ��ʂ�������
	}
	
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void UpdateEffect(void)
{
	int EffectCount = 0;				// ���������o���b�g�̐�



	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == TRUE)	// ���̃o���b�g���g���Ă���H
		{								// Yes

			g_Effect[i].span--;

			// �A�j���[�V����  
			g_Effect[i].countAnim++;
			if ((g_Effect[i].countAnim % ANIM_WAIT) == 0)
			{
				// �p�^�[���̐؂�ւ�
				g_Effect[i].patternAnim = (g_Effect[i].patternAnim + 1) % ANIM_PATTERN_NUM;
			}

			if(g_Effect[i].span < 1)
				g_Effect[i].use = FALSE;

			EffectCount++;
		}
	}


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEffect(void)
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

	SetBlendState(BLEND_MODE_ADD);		// ���Z����

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == TRUE)		// ���̃o���b�g���g���Ă���H
		{									// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_Effect[i].texNo]);

			//�o���b�g�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = g_Effect[i].pos.x;	// �o���b�g�̕\���ʒuX
			float py = g_Effect[i].pos.y;	// �o���b�g�̕\���ʒuY
			float pw = g_Effect[i].w;		// �o���b�g�̕\����
			float ph = g_Effect[i].h;		// �o���b�g�̕\������

            int frame = (int)(g_Effect[i].span * 0.12f);

			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
			float th = 1.0f;	// �e�N�X�`���̍���
			float tx = (float)(frame % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
			float ty = 0.0f;	// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColorRotation(g_VertexBuffer, 
				px, py, pw, ph, 
				tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 0.75f),
				g_Effect[i].rot.z);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	SetBlendState(BLEND_MODE_ALPHABLEND);	// ���������������ɖ߂�

}


//=============================================================================
// �o���b�g�\���̂̐擪�A�h���X���擾
//=============================================================================


//=============================================================================
// �o���b�g�̔��ːݒ�
//=============================================================================
void SetEffect(XMFLOAT3 pos)
{
	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		if (g_Effect[i].use == FALSE)		// ���g�p��Ԃ̃o���b�g��������
		{
			g_Effect[i].use = TRUE;			// �g�p��Ԃ֕ύX����
			g_Effect[i].pos = pos;			// ���W���Z�b�g
			g_Effect[i].span = EFFECT_SPAN;

			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}
