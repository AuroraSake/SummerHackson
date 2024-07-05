#include "Application.h"


Application::Application(const Window& window)
    : m_Window(window) 
{


}

void Application::Run()
{
    timeBeginPeriod(1);
    m_OldTickTime = m_NowTickTime = timeGetTime();

    while (true)
    {
        m_NowTickTime = timeGetTime();

        if (!m_Window.Update())
            break;
        if ((m_NowTickTime - m_OldTickTime) < pram_TickLength);
            continue;

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

}