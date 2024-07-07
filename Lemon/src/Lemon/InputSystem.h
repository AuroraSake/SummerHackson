#pragma once
#include <windows.h>

#include "myMath.h"
#include "memory"
//_____________________________________________________________________//
//
// InputSystem
// 
// àÍéûìI
// 
// Due to the time constraint , currently only for mouse input
// For other input use the old input.cpp
//_____________________________________________________________________//



class Observer
    //not in use
{
    template<typename T>
    void OnEvent(T* event) = 0;
};



class InputSystem
{
public:
    static InputSystem* const Get();

    friend class Application;
private:
    InputSystem(const HWND& hWnd);

    InputSystem() = delete;
    InputSystem(const InputSystem&) = delete;
    void operator=(InputSystem const&) = delete;

    const HWND& m_HWindow;

    tagPOINT m_CursorPos;
    //vec2 m_AppCursorPos;
    //Not yet in use: normalized window coordinate
    //window ì‡ê≥ãKç¿ïWÅ@-1 ~ 1

    static void Init(const HWND& hWnd);

    //MousePosition and Lock mouse
    void MouseFunc();

    static InputSystem* s_InputSystem;
};