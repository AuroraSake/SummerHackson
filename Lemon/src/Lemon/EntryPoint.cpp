#pragma once
#include "Application.h"
#include "Window.h"
//__Application_EntryPoint__________________________________________________________________//
//  2024/06/24--Ukoh
//  アプリケーション EntryPoint
//__________________________________________________________________________________________//



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Application app(Window(hInstance, nCmdShow));
    app.Run();

	// 終了処理
    return 0;
}