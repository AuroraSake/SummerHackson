#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include "myMath.h"
#include <string>
#include <vector>
#include <iostream>

class Sprite
{
    friend class SpriteManager;

private:
    vec2 m_UVstart;
    vec2 m_UVwidth;

    const ID3D11ShaderResourceView& m_rShaderResourceView;
protected:
    Sprite(const vec2& UVtop, const vec2& UVsize, const ID3D11ShaderResourceView& srv)
        : m_UVstart(UVtop), m_UVwidth(UVsize), m_rShaderResourceView(srv) {}
    ~Sprite();
};

class SpriteManager
{
public:
    const Sprite& CreatSprite(const vec2& UVtop, const vec2& UVsize, std::string fileName)
    {
        ID3D11ShaderResourceView* temp;
        HRESULT hResult;
        D3DX11CreateShaderResourceViewFromFile((ID3D11Device*)&m_device,
            fileName.c_str(),
            NULL,
            NULL,
            &temp,
            &hResult);
        if (!hResult)
            return;
        m_SpriteCount++;
        m_SpritePool.emplace_back(UVtop, UVsize, temp);
        return m_SpritePool[m_SpriteCount - 1];
        //m_SpriteCount‚Í“Y‚¦Žš‚æ‚è1‘å‚«‚¢?
        //m_SpriteCount should be 1 larger then the inserted element's index
    }

    //friend class SystemManager;
//protected:
    static void Init(const ID3D11Device& device)
    {
        if (s_SpriteManager)
            return;
        s_SpriteManager = new SpriteManager(device);
    }

    static const SpriteManager& GetSpriteManager()
    {
        return (SpriteManager&)s_SpriteManager;
    }


private:
    int m_SpriteCount = 0;
    const ID3D11Device& const m_device;
    static SpriteManager* s_SpriteManager;

    //sprite pool?
    //Learn use another memory allocation method later
    std::vector<Sprite> m_SpritePool;

    SpriteManager(const ID3D11Device& device)
        : m_device(device) 
    {
        m_SpritePool.reserve(20);//‰¼ƒTƒCƒY
    }
};