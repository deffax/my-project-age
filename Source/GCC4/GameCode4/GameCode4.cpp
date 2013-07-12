#include "GameCodeStd.h"
#include "GameCode.h"

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")

#pragma comment(lib, "tinyxml.lib")
#pragma comment(lib, "zlibstat.lib")

#pragma comment(lib, "dxut.lib")
#pragma comment(lib, "dxutopt.lib")

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "dxerr.lib")


#pragma comment(lib, "D3dx9.lib")

#pragma comment(lib, "D3DX11.lib")




INT WINAPI GameCode4(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
	Logger::Init("Logging.xml");
	
	g_pApp->m_Options.Init("PlayerOptions.xml", lpCmdLine);
	
	
	
		DXUTSetCallbackD3D9DeviceAcceptable( GameCodeApp::IsD3D9DeviceAcceptable );
		DXUTSetCallbackD3D9DeviceCreated( GameCodeApp::OnD3D9CreateDevice );
		DXUTSetCallbackD3D9DeviceReset( GameCodeApp::OnD3D9ResetDevice );
		DXUTSetCallbackD3D9DeviceLost( GameCodeApp::OnD3D9LostDevice );
		DXUTSetCallbackD3D9DeviceDestroyed( GameCodeApp::OnD3D9DestroyDevice );
		DXUTSetCallbackD3D9FrameRender( GameCodeApp::OnD3D9FrameRender );
	
	

    // Show the cursor and clip it when in full screen
    DXUTSetCursorSettings( true, true );
	DXUTMainLoop();
	DXUTShutdown();
	Logger::Destroy();
    return 0;	
}

