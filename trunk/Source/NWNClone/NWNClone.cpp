#include "NWNCloneStd.h"
#include "../GCC4/GameCode4/GameCode.h"

#include "NWNClone.h"

NWNCloneApp g_NWNCloneApp;


INT WINAPI wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
	return GameCode4(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}