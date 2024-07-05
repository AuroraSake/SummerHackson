#pragma once
#include "myMath.h"
//_____________________________________________________________________//
//
// Transform.h
// 
// WIP版
// 
// 作業効率が良くするために作った
// Transform component に当たるもの
//
// To do:
//  transform system を作ってtransform component を管理する
//  object pool パターンで連続メモリー空間で格納する
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
