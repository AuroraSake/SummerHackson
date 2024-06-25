//=============================================================================
//
// �v���C���[���� [player.h]
// Author : GP11B132 99 �O������
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SetSprite(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH);

void SetSpriteColor(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH, XMFLOAT4 color);

void SetSpriteColorRotation(ID3D11Buffer *buf, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color, float Rot);

void SetSpriteLeftTop(ID3D11Buffer *buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH);

void SetSpriteFreeVertex(ID3D11Buffer *buf, XMFLOAT3 pos , float rot,
	XMFLOAT4 Color, XMFLOAT4 vertices[], int vertexNum);