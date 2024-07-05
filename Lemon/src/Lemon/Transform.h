#pragma once
#include "myMath.h"
//_____________________________________________________________________//
//
// Transform.h
// 
// WIP��
// 
// ��ƌ������ǂ����邽�߂ɍ����
// Transform component �ɓ��������
//
// To do:
//  transform system �������transform component ���Ǘ�����
//  object pool �p�^�[���ŘA���������[��ԂŊi�[����
//_____________________________________________________________________//


class Transform
{
public:
    Vector3 m_position = 0.0f;
    Vector3 m_rotation = 0.0f;
    Vector3 m_scale    = 1.0f;

    Vector3 m_forward = (0.0f, 0.0f, 1.0f);
    Vector3 m_up      = (0.0f, 1.0f, 0.0f);

    DirectX::XMMATRIX m_mtxModel = DirectX::XMMatrixIdentity();

    void UpdateMMatrix();
    void UpdateDirection();
};
