#pragma once
#include "myMath.h"
#include <string>
#include <iostream>
#include <vector>
//_____________________________________________________________________//
//
// Transform
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
    Transform() = default;
    Transform(const char* name);
    //Transform(Transform* other);

    Vector3 position = 0.0f;
    Vector3 rotation = 0.0f;
    Vector3 scale = 1.0f;

    Vector3 forward = Vector3(0.0f, 0.0f, 1.0f);
    Vector3 up = Vector3(0.0f, 1.0f, 0.0f);

    DirectX::XMMATRIX mtxModel = DirectX::XMMatrixIdentity();
    DirectX::XMMATRIX mtxWorld = DirectX::XMMatrixIdentity();

    Transform* Parent() const;
    void SetParent(Transform& parent);
    void SetParent(Transform* parent);
    //set self or Null as parent would remove the parent
    //parentをクリア出来るには：自身かNull を設定する

    void Print();
    void PrintPos();
    void PrintChild();
    void Rename(const char* name);

private:
    bool m_InUse = false;
    bool m_LateUpdate = false;
    std::string name = "Null";

    Transform* m_pParent = nullptr;
    Transform* m_pChildHead = nullptr;
    Transform* m_pChildTail = nullptr;
    Transform* m_pNext = nullptr;
    Transform* m_pLast = nullptr;

    void RemoveChild(Transform& child);
    
    void UpdateModelMatrix();
    void UpdateWorldMatrix();
    void UpdateDirection();
    void Update();
    void LateUpdate();
};
