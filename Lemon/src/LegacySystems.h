#pragma  once
#include "Window.h"

//旧仕様から新アーキテクチャに移行する途中の一時仕様

void SetMode(int mode);
void Update();
void Draw();
void Init(const HINSTANCE& instance, const HWND& hWnd, bool isWindowed, int defaultMode);
void Uninit();
int  GetMode();