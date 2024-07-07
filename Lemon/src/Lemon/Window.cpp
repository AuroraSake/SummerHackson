#pragma  once
#include "Window.h"
#include "string"

//static WndProc の message 転送が実装出来るまでの一時仕様
#include "input.h"

const static WNDCLASSEX defaultWindowClass = 
{
    sizeof(WNDCLASSEX),
    CS_CLASSDC,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    LoadCursor(NULL, IDC_ARROW),
    (HBRUSH)(COLOR_WINDOW + 1),
    NULL,
    NULL,
    NULL
};


Window::Window(const HINSTANCE& instance, int showWindow)
    : m_HNowInstance(instance), m_ShowWindow(showWindow) {}

Window::~Window()
{
    UnregisterClass(m_WindowPrams.className.c_str(), m_HNowInstance);
}


bool Window::CreatWindow(const WNDCLASSEX* const _windowClass)
{
    WNDCLASSEX	windowClass = (_windowClass) ? *_windowClass : defaultWindowClass;
    windowClass.lpfnWndProc = WndProc;
    windowClass.hInstance = m_HNowInstance;
    windowClass.lpszClassName = m_WindowPrams.className.c_str();
    bool result = RegisterClassEx((const WNDCLASSEXA*)&windowClass);
        
    m_HWindow = CreateWindowA(
        m_WindowPrams.className.c_str(),
        m_WindowPrams.windowName.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,//Upper-Left corner in: x, y 
        m_WindowPrams.windowSize.x + GetSystemMetrics(SM_CXDLGFRAME),
        m_WindowPrams.windowSize.y + GetSystemMetrics(SM_CXDLGFRAME) + GetSystemMetrics(SM_CYCAPTION),
        NULL,
        NULL,
        m_HNowInstance,
        NULL);

    if (!m_HWindow)
        return 0;
    return 1;
}

void Window::Show()
{
    ShowWindow(m_HWindow, m_ShowWindow);
}

bool Window::Update()
{
    if (m_HideCursor)
        while (ShowCursor(false) >= 0);
    else
        while (ShowCursor(true) < 0);

    MSG windowMessage;
    if (PeekMessage(&windowMessage, NULL, 0, 0, PM_REMOVE))
    {
        if (windowMessage.message == WM_QUIT)
            return 0;
        else
        {
            TranslateMessage(&windowMessage);
            DispatchMessage(&windowMessage);
        }
    }
    return 1;
}


LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    //static WndProc の message 転送が実装出来たらこちらを使う
    //case WM_LBUTTONDOWN:
    //    m_HideCursor = true;
    //    break;
    //case WM_KEYDOWN:
    //    switch (wParam)
    //    {
    //    case VK_ESCAPE:
    //        m_HideCursor = false;
    //        break;
    //    }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}