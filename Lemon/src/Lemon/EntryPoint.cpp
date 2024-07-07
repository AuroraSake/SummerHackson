#pragma once
#include "Application.h"
#include "Window.h"
//__Application_EntryPoint__________________________________________________________________//
//
// 2024/06/24--Ukoh
// アプリケーション EntryPoint
// 
// 
// このremake/refactor は、プログラムの理解を深めるため、
// アーキテクチャを考察し、勉強しつつ、将来のプログラム実験場の基盤とするシステムの構築に当たります
// 
// To do:
//  Learn,
//  and git gud.
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