#pragma  once
#include "myMath.h"
#include "Transform.h"

#include <sstream>

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


//Transform::Transform(Transform& other)
//{
//    SetParent(&other);
//};

Transform::Transform(const char* _name)
    : name(_name) {}


void Transform::UpdateModelMatrix()
{
    mtxModel = DirectX::XMMatrixAffineTransformation(
        scale.XMVECTOR(),
        Vector3(0.0f).XMVECTOR(),
        DirectX::XMQuaternionRotationRollPitchYawFromVector(rotation.XMVECTOR()),
        position.XMVECTOR()
    );
}

void Transform::UpdateWorldMatrix()
{
    DirectX::XMMATRIX temp = mtxModel;
    temp.r[3] = { 0.0f, 0.0f, 0.0f, 1.0f };
    Transform** pNode = &(this->m_pParent);
    
    while(*pNode)
    {
        if ((*pNode)->m_LateUpdate)
        {
            temp = (*pNode)->mtxWorld * temp;
            break;
        }
        temp = (*pNode)->mtxModel * temp;
        pNode = &((*pNode)->m_pParent);
    }
    mtxWorld = temp;
}


void Transform::UpdateDirection()
{
    forward = Vector3(0.0f, 0.0f, 1.0f).Rotate(rotation);
    up      = Vector3(0.0f, 1.0f, 0.0f).Rotate(rotation);
}


void Transform::Update()
{
    UpdateModelMatrix();
    m_LateUpdate = false;
}

void Transform::LateUpdate()
{
    UpdateWorldMatrix();
    m_LateUpdate = true;
}

Transform* Transform::Parent() const
{
    return m_pParent;
}

void Transform::SetParent(Transform* parent)
{
    if ( parent == this || parent == nullptr)
    {
        if (m_pParent)
            m_pParent->RemoveChild(*(this));
        return;
    }
    return SetParent(*parent);
}   

void Transform::SetParent(Transform& parent)
{
    if (m_pParent == &parent)
        return;

    if (m_pParent)
        m_pParent->RemoveChild(*(this));
    
    if (&parent == this)
        return;

    this->m_pLast = parent.m_pChildTail;

    if(!parent.m_pChildHead)
        parent.m_pChildHead = this;
    if(parent.m_pChildTail)
        parent.m_pChildTail->m_pNext = this;
    parent.m_pChildTail = this;

    this->m_pParent = &parent;
}

void Transform::Print()
{
    std::cout << name << std::endl;
}

void Transform::PrintPos()
{
    Update();
    UpdateWorldMatrix();
    (position * mtxWorld).print();
}

void Transform::PrintChild()
{
    std::string childList = "ChildList:\n";

    Transform** pNode = &(this->m_pChildHead);
    while ((*pNode))
    {
        childList += (*pNode)->name + "\n";
        pNode = &((*pNode)->m_pNext);
    }
    std::cout << childList << std::endl;
}

void Transform::Rename(const char* _name)
{
    name = _name;
}

void Transform::RemoveChild(Transform& child)
{
    if(m_pChildHead == &child)
        m_pChildHead = child.m_pNext;
    if(m_pChildTail == &child)
        m_pChildTail = child.m_pLast;

    if(child.m_pLast)
    child.m_pLast->m_pNext = child.m_pNext;
    if(child.m_pNext)
    child.m_pNext->m_pLast = child.m_pLast;

    child.m_pParent = child.m_pNext = child.m_pLast = nullptr;
}