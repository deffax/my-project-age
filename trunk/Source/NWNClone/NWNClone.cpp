#include "NWNCloneStd.h"
#include "../GCC4/GameCode4/GameCode.h"
#include "../GCC4/Mainloop/Initialization.h"

#include "NWNClone.h"

NWNCloneApp g_NWNCloneApp;


INT WINAPI wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
	return GameCode4(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}


HICON NWNCloneApp::VGetIcon()
{
	return LoadIcon(GetInstance(), 0);
}

BaseGameLogic* NWNCloneApp::VCreateGameAndView()
{
	m_pGame = GCC_NEW NWNCloneLogic();
	return m_pGame;
}