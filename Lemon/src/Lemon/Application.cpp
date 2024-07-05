#pragma  once
#include "Application.h"
#include "src/LegacySystems.h"
//_____________________________________________________________________//
//
// Application.cpp
// 
// Game Application のメインループおよび処理など
// 
// Window class の instance も一個管理している
// 
// To do:
//  Core System (Input, renderer など)の旧式からの更新、組み込み&疎結合
//  OpenGLサポート向けてGLFWの導入
//  開発便利性のためのIMGUIの導入
//_____________________________________________________________________//





Application::Application(const Window& window)
    : m_Window(window), m_DTime(0), m_NowTickTime(0), m_OldTickTime(0) {}

Application::~Application()
{
    Uninit();
}

void Application::Run()
{
    if (!m_Window.CreatWindow(NULL))
        return;
    m_Window.Show();
    Init(m_Window.m_HNowInstance, m_Window.m_HWindow, true, pram_DefaultMode);


    timeBeginPeriod(1);
    m_OldTickTime = m_NowTickTime = timeGetTime();

    while (true)
    {
        if (!m_Window.Update())
            break;
        if ((m_NowTickTime - m_OldTickTime) < pram_TickLength)
            continue;

        m_NowTickTime = timeGetTime();
        m_OldTickTime = m_NowTickTime;
        m_DTime = (m_NowTickTime - m_OldTickTime) * 0.001f;

        Tick();
    }
    timeEndPeriod(1);
}

void Application::Tick()
{
    //use this for none rendering update//
    //physics etc
    Update();
    Draw();
}