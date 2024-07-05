#pragma once
#include "windows.h"
#include "string"

struct WindowPram
{
    std::string className = "Fruit_Class";
    std::string windowName = "Fruit";
    tagPOINT windowSize = { 1600, 900 };
};



class Window
{
public:
    const bool m_hideCursor = true;

    Window(const HINSTANCE& instance, int showWindow);
    ~Window();

    const HWND& GetHandle()
    {
        return m_HWindow;
    }

    //pass null for default window
    bool CreatWindow(const WNDCLASSEX* const _windowClass);
    void Show();
    bool Update();

    friend class Application;
private:
    HWND m_HWindow;
    HINSTANCE m_HNowInstance;
    LPSTR m_CommandLine;
    int m_ShowWindow;
    
    WindowPram m_WindowPrams;
    bool m_Windowed;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};