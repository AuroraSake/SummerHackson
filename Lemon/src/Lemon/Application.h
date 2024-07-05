#pragma once
#include <Windows.h>
#include "mmsystem.h"
#include <timeapi.h>

#include "Window.h"
#include "AppEntry.h"



class Application
{
public:
    Application(const Window& window)
        : m_Window(window) {}
    ~Application() {}

    void Run();
    void Tick();

private:
    const uint64_t pram_TickLength = 1000 / 60;

    Window m_Window;

    float    m_DTime;
    uint64_t m_OldTickTime;
    uint64_t m_NowTickTime;
};