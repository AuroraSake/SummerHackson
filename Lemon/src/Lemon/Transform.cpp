#pragma  once
#include "myMath.h"
#include "Transform.h"

//_____________________________________________________________________//
//
// Transform
// 
// 
// 作業効率が良くするために作った
// Transform component に当たるもの
//
// To do:
//  transform system を作ってtransform component を管理する
//  object pool パターンで連続メモリー空間で格納する
//_____________________________________________________________________//


Transform::Transform(const Transform* other)
    : m_pParent(other) {};



void Transform::UpdateMMatrix()
{
    m_mtxModel = DirectX::XMMatrixAffineTransformation(
        m_scale.XMVECTOR(),
        Vector3(0.0f).XMVECTOR(),
        DirectX::XMQuaternionRotationRollPitchYawFromVector(m_rotation.XMVECTOR()),
        m_position.XMVECTOR()
    );
}


void Transform::UpdateDirection()
{
    m_forward = Vector3(0.0f, 0.0f, 1.0f).Rotate(m_rotation);
    m_up      = Vector3(0.0f, 1.0f, 0.0f).Rotate(m_rotation);
}


void Transform::Update()
{
    UpdateDirection();
    UpdateMMatrix();
}