#pragma once
#include <vector>
#include <unordered_map>


#include "GameObject.h"
#include "Component/Component.h"

namespace std
{
    template<>
    struct hash<GameObject>
    {
        size_t operator()(const GameObject& key)
        {
            return hash<uint64_t>()((uint64_t)&key);
        }
        size_t operator()(const GameObject* key)
        {
            return hash<uint64_t>()((uint64_t)key);
        }
    };
}


class Scene
{
};


class SceneManager
{
private:
    std::vector<GameObject> m_EntityPool;
};

