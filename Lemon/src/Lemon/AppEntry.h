//__Application_EntryPoint__________________________________________________________________//
//  2024/06/24--Ukoh
//  アプリケーション EntryPoint
//__________________________________________________________________________________________//
#pragma once
#include <time.h>

#include "Application.h"
#include "Window.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// 時間計測用
    Window window(hInstance, nCmdShow);
    if (!window.CreatWindow(NULL))
        return -1;
    window.Show();


	// 終了処理
	Uninit();
    return 0;
}