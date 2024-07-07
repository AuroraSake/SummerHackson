#pragma once
#include <Windows.h>
#include "mmsystem.h"
#include <timeapi.h>

#include "Window.h"
#include "src/LegacySystems.h"
//_____________________________________________________________________//
//
// Application
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

class Application
{
public:
    Application(const Window& window);
    ~Application();

    void Run();
    void Tick();

private:
    const uint64_t pram_TickLength = 1000 / 60;
    const int pram_DefaultMode = 0;
    //remember to change this to something that can load from a config file
    //初期設定など、余裕があったら設定ファイルで読み込まれるようなシステムにしよう

    Window m_Window;

    float    m_DTime       = 0;
    uint64_t m_OldTickTime = 0;
    uint64_t m_NowTickTime = 0;
};