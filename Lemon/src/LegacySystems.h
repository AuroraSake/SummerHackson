#pragma  once
#include "Window.h"

//���d�l����V�A�[�L�e�N�`���Ɉڍs����r���̈ꎞ�d�l

void SetMode(int mode);
void Update();
void Draw();
void Init(const HINSTANCE& instance, const HWND& hWnd, bool isWindowed, int defaultMode);
void Uninit();
int  GetMode();