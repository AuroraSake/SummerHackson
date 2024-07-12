#pragma once
#include "Transform.h"
#include <vector>

class ObjectPool
{
    std::vector<GameObject> s_GameObjectPool;
};

class Component
{

private:
    Component* m_pNext;
    Component* m_pLast;
};

class GameObject
{
    Transform m_transform;

    void* const m_pData;

    Component* m_pComponentListHead;
    Component* m_pComponentListTail;

    virtual void Update();
    virtual void LateUpdate();
};

