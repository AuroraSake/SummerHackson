/*******************************************************************************
* �^�C�g��:		�^�C�}�[�v���O����
* �v���O������:	timer.cpp
* �쐬��:		GP11A132 21 �쎛�Č�
* �쐬��:		2023/07/28
*******************************************************************************/



/*******************************************************************************
* �C���N���[�h�t�@�C��
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
* �}�N����`
*******************************************************************************/
#define TEXTURE_WIDTH				(16)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(32)	// 
#define TEXTURE_MAX					(1)		// �e�N�X�`���̐�

/*******************************************************************************
* �\���̒�`
*******************************************************************************/


/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/


/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[] = {
	"data/TEXTURE/number_score.png",
};

int curtime;

static bool						g_Use;						// true:�g���Ă���  false:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

static int						g_Timer;					// �^�C�}�[

DWORD g_timerStart;


//int maxtimer = TIMER_MAX;
/*******************************************************************************
 �֐���:	
 �����@:	
 �߂�l:	
 �����@:	
*******************************************************************************/


//=============================================================================
// ����������
//=============================================================================
HRESULT InitTimer(void)
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
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �v���C���[�̏�����
	g_Use = true;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = { 500.0f, 20.0f, 0.0f };
	g_TexNo = 0;
	curtime = TIMER_MAX;

	return S_OK;


}


//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void UpdateTimer(void)
{

	if ((int)timeGetTime() - (int)g_timerStart >= 1 * 1000)//�~���b�ɂ���̂�1000�{
	{
		g_timerStart = timeGetTime();
		//curtime = maxtimer - (g_timerStart / 1000);
		curtime--;
	}


#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawTimer(void)
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
    //if curtime <
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);


	// ��������������
	int number = curtime;
	//number = 20;
	for (int i = 0; i < TIMER_DIGIT; i++)
	{
		// ����\�����錅�̐���
		float x = number;
		//float x = 19;

		// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
		float px = g_Pos.x - g_w * i;	// �X�R�A�̕\���ʒuX
		float py = g_Pos.y;			// �X�R�A�̕\���ʒuY
		float pw = g_w;				// �X�R�A�̕\����
		float ph = g_h;				// �X�R�A�̕\������

		float tw = 1.0f / 10;		// �e�N�X�`���̕�
		float th = 1.0f / 1;		// �e�N�X�`���̍���
		float tx = 0;// = x * tw;			// �e�N�X�`���̍���X���W 
		//if (i == 0)
		//{
			tx = (int)x % 10 * tw;
		//}
		//else if (i == 1)
		//{
		//	//tx = (int)x % 10 * tw;
		//	tx = x / 10 * tw;
		//}
		float ty = 0.0f;			// �e�N�X�`���̍���Y���W
        XMFLOAT4 diffuse = {1.0f, 1.0f, 1.0f, 1.0f};
        if (curtime < 10)
            diffuse = {1.0f, 0.0f, 0.0f, 1.0f};


		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
            diffuse);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

		// ���̌���
		number /= 10;
	}

}


//=============================================================================
// �^�C�}�[�����Z����
// ����:add :�ǉ�����_���B�}�C�i�X���\
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