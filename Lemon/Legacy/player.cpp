//=============================================================================
//
// �v���C���[���� [player.cpp]
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
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

#define CURSORSIZE   				(30)	// �L�����T�C�Y
#define TEXTURE_MAX					(1)		// �e�N�X�`���̐�


#define PLAYER_MAX		                    (1)		// �v���C���[��Max�l��
#define ATTACK_COOLDOWN                     (10)
#define CURSORSPEED                         (10.0f)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[TEXTURE_MAX] = {
    "data/TEXTURE/cursor.png",
};


static BOOL		g_Load = FALSE;			// ���������s�������̃t���O

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
// ����������
//=============================================================================
HRESULT InitPlayer(void)
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


    CursorPos = { SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0};
    mouseX = 0;
    mouseY = 0;
    oldMousePos = { mouseX, mouseY, 0.0 };

    g_Load = TRUE;
    return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
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

    if (CursorPos.y > SCREEN_HEIGHT)    // �����̑傫�����l�����ĉ�ʊO�����肵�Ă���
    {
        CursorPos.y = SCREEN_HEIGHT;
    }
    if (CursorPos.y < 0.0)              // �����̑傫�����l�����ĉ�ʊO�����肵�Ă���
    {
        CursorPos.y = 0.0;
    }
    if (CursorPos.x < 0.0)              // �����̑傫�����l�����ĉ�ʊO�����肵�Ă���
    {
        CursorPos.x = 0.0;
    }
    if (CursorPos.x > SCREEN_WIDTH)              // �����̑傫�����l�����ĉ�ʊO�����肵�Ă���
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
#ifdef _DEBUG	// �f�o�b�O����\������


#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
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

    // �e�N�X�`���ݒ�
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

    //�v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
    float px = CursorPos.x;	// �v���C���[�̕\���ʒuX
    float py = CursorPos.y;	// �v���C���[�̕\���ʒuY
    float pw = CURSORSIZE;		// �v���C���[�̕\����
    float ph = CURSORSIZE;		// �v���C���[�̕\������

    // �A�j���[�V�����p
    //float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
    //float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
    //float tx = (float)(g_Player[i].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
    //float ty = (float)(g_Player[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

    float tw = 1.0f;	// �e�N�X�`���̕�
    float th = 1.0f;							// �e�N�X�`���̍���
    float tx = tw;	// �e�N�X�`���̍���X���W
    float ty = 0.0f;							// �e�N�X�`���̍���Y���W

    // �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
    SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
    	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // �|���S���`��
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
// Player�\���̂̐擪�A�h���X���擾
//=============================================================================}




