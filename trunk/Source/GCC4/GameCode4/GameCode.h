#pragma once
#include "../Mainloop/Initialization.h"

class GameCodeApp
{
public:
	GameCodeApp();

	virtual TCHAR* VGetGameTitle() = 0;

	virtual bool InitInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd = NULL,
		int screenWidth = SCREEN_WIDTH, int screenHeight = SCREEN_HEIGHT);
};


extern GameCodeApp* g_pApp;