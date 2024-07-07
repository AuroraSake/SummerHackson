#include "InputSystem.h"
//_____________________________________________________________________//
//
// InputSystem
// 
// 一時的
// マウス操作だけ
// 後でメッセージシステムに切り替える
// Due to the time constraint , currently only for mouse input
// For other input use the old input.cpp
//_____________________________________________________________________//


InputSystem* InputSystem::s_InputSystem = nullptr;

InputSystem* const InputSystem::Get()
{
    if (s_InputSystem)
        return s_InputSystem;
}

void InputSystem::Init(const HWND& hWnd)
{
    if (s_InputSystem)
        return;
    s_InputSystem = new InputSystem(hWnd);
}

//MousePosition and Lock mouse
void InputSystem::MouseFunc()
{
    if (GetForegroundWindow() != m_HWindow)
        return;

    RECT rect;
    GetWindowRect(m_HWindow, &rect);

    GetCursorPos(&m_CursorPos);
    int mouseX = m_CursorPos.x; int mouseY = m_CursorPos.y;
    tagPOINT cursorResetPos = m_CursorPos;

    bool inWnd = true;
    {
        int tolerance = 10;
        if (mouseX < rect.left - tolerance)
        {
            cursorResetPos.x = rect.left;
            inWnd = false;
        }
        else if (mouseX > rect.right + tolerance)
        {
            cursorResetPos.x = rect.right;
            inWnd = false;
        }

        if (mouseY < rect.top - tolerance)
        {
            cursorResetPos.y = rect.top;
            inWnd = false;
        }
        else if (mouseY > rect.bottom + tolerance)
        {
            cursorResetPos.y = rect.bottom;
            inWnd = false;
        }
    }
    if (!inWnd) SetCursorPos(cursorResetPos.x, cursorResetPos.y);

    m_CursorPos.x = mouseX - rect.left;
    m_CursorPos.y = -(mouseY - rect.top);
}