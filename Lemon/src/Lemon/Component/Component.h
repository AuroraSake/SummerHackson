#pragma once
#include <vector>
#include <unordered_map>
#include "GameObject.h"
#include "Transform.h"




class ComponentPool
{
public:
    ComponentPool()
    {
        m_Pool.reserve(10);
    }

private:
    std::vector<Component>  m_Pool;
    std::unordered_map<GameObject, Component&> m_RefMap;

    void RegisterComponent(const GameObject& owner)
    {
        if (owner.m_ComponentMask | (uint64_t)Component::Mask())
            return;//prevent duplicate

        //to do: add freelist keep tracking free slots
        if (m_Pool.size() < m_Pool.capacity())
        {
            m_Pool.emplace_back();
            m_RefMap[owner] = m_Pool.back();
            return;
        }

        {
            int count = 0;
            for (Component& element : m_Pool)
            {
                if (!element.free)
                    continue;
            }
        }
    }
};


enum class ComponentMask
{
    Null = -1,
    Transform,
};


class Component
{
public:
    static ComponentMask Mask();

    friend class ComponentPool;
private:
    bool free = false;

    virtual void OnUpdate();
    virtual void OnLateUpdate();
    virtual void OnRender();
};

