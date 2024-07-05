//__Application_EntryPoint__________________________________________________________________//
//  2024/06/24--Ukoh
//  �A�v���P�[�V���� EntryPoint
//__________________________________________________________________________________________//
#pragma once
#include <time.h>

#include "Application.h"
#include "Window.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// ���Ԍv���p
    Window window(hInstance, nCmdShow);
    if (!window.CreatWindow(NULL))
        return -1;
    window.Show();


	// �I������
	Uninit();
    return 0;
}