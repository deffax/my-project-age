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
//#pragma comment(lib, "dxguid.lib")




INT WINAPI GameCode4(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{

	
	//
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

	// set this flag to keep memory blocks around
	// tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;	       // this flag will cause intermittent pauses in your game and potientially cause it to run out of memory!

	// perform memory check for each alloc/dealloc
	//tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;		   // remember this is VERY VERY SLOW!

	//_CRTDBG_LEAK_CHECK_DF is used at program initialization to force a 
	//   leak check just before program exit. This is important because
	//   some classes may dynamically allocate memory in globally constructed
	//   objects.
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;					

	_CrtSetDbgFlag(tmpDbgFlag);
	Logger::Init("Logging.xml");

	
	g_pApp->m_Options.Init("PlayerOptions.xml", lpCmdLine);
	
	if(g_pApp->m_Options.m_Renderer== "Direct3D 9")
	{
		DXUTSetCallbackD3D9DeviceAcceptable( GameCodeApp::IsD3D9DeviceAcceptable );
		DXUTSetCallbackD3D9DeviceCreated( GameCodeApp::OnD3D9CreateDevice );
		DXUTSetCallbackD3D9DeviceReset( GameCodeApp::OnD3D9ResetDevice );
		DXUTSetCallbackD3D9DeviceLost( GameCodeApp::OnD3D9LostDevice );
		DXUTSetCallbackD3D9DeviceDestroyed( GameCodeApp::OnD3D9DestroyDevice );
		DXUTSetCallbackD3D9FrameRender( GameCodeApp::OnD3D9FrameRender );
	}
	else if (g_pApp->m_Options.m_Renderer== "Direct3D 11")
	{
		DXUTSetCallbackD3D11DeviceAcceptable( GameCodeApp::IsD3D11DeviceAcceptable );
		DXUTSetCallbackD3D11DeviceCreated( GameCodeApp::OnD3D11CreateDevice );
		DXUTSetCallbackD3D11SwapChainResized( GameCodeApp::OnD3D11ResizedSwapChain );
		DXUTSetCallbackD3D11SwapChainReleasing( GameCodeApp::OnD3D11ReleasingSwapChain );
		DXUTSetCallbackD3D11DeviceDestroyed( GameCodeApp::OnD3D11DestroyDevice );
		DXUTSetCallbackD3D11FrameRender( GameCodeApp::OnD3D11FrameRender );	
	}
	else
	{
		GCC_ASSERT(0 && "Unknown renderer specified in game options.");
		return false;
	}
	
	
	

    // Show the cursor and clip it when in full screen
    DXUTSetCursorSettings( true, true );

	if(!g_pApp->InitInstance(hInstance, lpCmdLine, 0,
		g_pApp->m_Options.m_ScreenSize.x,
		g_pApp->m_Options.m_ScreenSize.y))
	{
		return FALSE;
	}


	DXUTMainLoop();
	DXUTShutdown();
	Logger::Destroy();
	return g_pApp->GetExitCode();	
}

