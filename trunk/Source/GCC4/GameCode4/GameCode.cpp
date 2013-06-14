#include "GameCodeStd.h"
#include "../Mainloop/Initialization.h"

GameCodeApp* g_pApp = NULL;

GameCodeApp::GameCodeApp()
{
	g_pApp = this;
}

bool GameCodeApp::InitInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd,
		int screenWidth, int screenHeight)
{
#ifndef _DEBUG
	if(!IsOnlyInstance(VGetGameTitle()))
		return false;
#endif

	SetCursor(NULL);

	bool resourceCheck = false;
	while(!resourceCheck)
	{
		const DWORDLONG physicalRAM = 512 * MEGABYTE;
		const DWORDLONG virtualRAM = 1024 * MEGABYTE;
		const DWORDLONG diskSpace = 10 * MEGABYTE;

		if(!CheckStorage(diskSpace))
			return false;

		const DWORD minCPUSpeed = 1300;
		DWORD thisCPU = ReadCPUSpeed();
		if(thisCPU < minCPUSpeed)
		{
			GCC_ERROR("GetCPUSpeed reports CPU is too slow for this game.");
			return false;
		}
		resourceCheck = true;
	}


	

	return true; //PROVVISORIO!!!!
}