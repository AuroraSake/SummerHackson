#pragma  once
#include "myMath.h"
#include "Transform.h"

#include <sstream>

//_____________________________________________________________________//
//
// Transform
// 
// 
// ��ƌ������ǂ����邽�߂ɍ����
// Transform component �ɓ��������
//
// To do:
//  transform system �������transform component ���Ǘ�����
//  object pool �p�^�[���ŘA���������[��ԂŊi�[����
//_____________________________________________________________________//


//Transform::Transform(Transform& other)
//{
//    SetParent(&other);
//};

Transform::Transform(const char* _name)
    : name(_name) {}


void Transform::UpdateModelMatrix()
{
    m_mtxModel = DirectX::XMMatrixAffineTransformation(
        m_scale.XMVECTOR(),
        Vector3(0.0f).XMVECTOR(),
        DirectX::XMQuaternionRotationRollPitchYawFromVector(m_rotation.XMVECTOR()),
        m_position.XMVECTOR()
    );
}


DirectX::XMMATRIX Transform::WorldMatrix()
{
    DirectX::XMMATRIX temp = m_mtxModel;
    temp.r[3] = { 0.0f, 0.0f, 0.0f, 1.0f };
    Transform** pNode = &(this->m_pParent);
    while(*pNode)
    {
        temp = (*pNode)->m_mtxModel * temp;
        pNode = &((*pNode)->m_pParent);
    }
    return temp;
}


void Transform::UpdateDirection()
{
    m_forward = Vector3(0.0f, 0.0f, 1.0f).Rotate(m_rotation);
    m_up      = Vector3(0.0f, 1.0f, 0.0f).Rotate(m_rotation);
}


void Transform::Update()
{
    UpdateModelMatrix();
}

const Transform* Transform::Parent() const
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
    DirectX::XMMATRIX temp = WorldMatrix();
    (m_position * WorldMatrix()).print();
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