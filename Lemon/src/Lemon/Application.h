#pragma once
#include <Windows.h>
#include "mmsystem.h"
#include <timeapi.h>

#include "Window.h"
#include "src/LegacySystems.h"
//_____________________________________________________________________//
//
// Application
// 
// Game Application �̃��C�����[�v����я����Ȃ�
// 
// Window class �� instance ����Ǘ����Ă���
// 
// To do:
//  Core System (Input, renderer �Ȃ�)�̋�������̍X�V�A�g�ݍ���&�a����
//  OpenGL�T�|�[�g������GLFW�̓���
//  �J���֗����̂��߂�IMGUI�̓���
//_____________________________________________________________________//

class Application
{
public:
    Application(const Window& window);
    ~Application();

    void Run();
    void Tick();

private:
    const uint64_t pram_TickLength = 1000 / 60;
    const int pram_DefaultMode = 0;
    //remember to change this to something that can load from a config file
    //�����ݒ�ȂǁA�]�T����������ݒ�t�@�C���œǂݍ��܂��悤�ȃV�X�e���ɂ��悤

    Window m_Window;

    float    m_DTime       = 0;
    uint64_t m_OldTickTime = 0;
    uint64_t m_NowTickTime = 0;
};