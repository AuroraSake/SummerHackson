#pragma once
#include "Application.h"
#include "Window.h"
//__Application_EntryPoint__________________________________________________________________//
//
// 2024/06/24--Ukoh
// �A�v���P�[�V���� EntryPoint
// 
// 
// ����remake/refactor �́A�v���O�����̗�����[�߂邽�߁A
// �A�[�L�e�N�`�����l�@���A�׋����A�����̃v���O����������̊�ՂƂ���V�X�e���̍\�z�ɓ�����܂�
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

	// �I������
    return 0;
}