#pragma once
#include "Transform.h"
#include "Component/Component.h"
#include <vector>
#include <unordered_map>

class ObjectPool
{
    std::vector<GameObject> s_GameObjectPool;
};


class GameObject
{
public:

    template<class T>
    void AddComponent()
    {
        uint64_t mask = T::Mask();
        m_ComponentMask |= 1 << mask;
    }

    friend class ComponentPool;
private:
    uint64_t m_ComponentMask;

    virtual void Update();
    virtual void LateUpdate();
};

