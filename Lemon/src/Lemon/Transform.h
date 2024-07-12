#pragma once
#include "myMath.h"
#include <string>
#include <iostream>
//_____________________________________________________________________//
//
// Transform
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
    Transform() = default;
    Transform(const char* name);
    //Transform(Transform* other);

    Vector3 m_position = 0.0f;
    Vector3 m_rotation = 0.0f;
    Vector3 m_scale = 1.0f;

    Vector3 m_forward = Vector3(0.0f, 0.0f, 1.0f);
    Vector3 m_up = Vector3(0.0f, 1.0f, 0.0f);

    DirectX::XMMATRIX m_mtxModel = DirectX::XMMatrixIdentity();

    void UpdateModelMatrix();
    void UpdateDirection();
    void Update();

    DirectX::XMMATRIX WorldMatrix();

    const Transform* Parent() const;
    void SetParent(Transform& parent);
    void SetParent(Transform* parent);

    void Print();
    void PrintPos();
    void PrintChild();
    void Rename(const char* name);
private:
    std::string name = "Null";

    Transform* m_pParent = nullptr;
    Transform* m_pChildHead = nullptr;
    Transform* m_pChildTail = nullptr;
    Transform* m_pNext = nullptr;
    Transform* m_pLast = nullptr;

    void RemoveChild(Transform& child);
};
