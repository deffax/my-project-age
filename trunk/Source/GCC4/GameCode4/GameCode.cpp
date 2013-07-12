#include "GameCodeStd.h"
#include "../Mainloop/Initialization.h"
#include "../ResourceCache/ResCache.h"

GameCodeApp* g_pApp = NULL;

GameCodeApp::GameCodeApp()
{
	g_pApp = this;
	m_resCache = NULL;
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

	IResourceFile *zipFile = GCC_NEW ResourceZipFile(L"Assets.zip");
	m_resCache = GCC_NEW ResCache(50, zipFile);

	if (!m_resCache->Init())
	{
        GCC_ERROR("Failed to initialize resource cache!  Are your paths set up correctly?");
		return false;
	}
	

	return true; //PROVVISORIO!!!!
}

//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK GameCodeApp::OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice,
                                    const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
   
    return S_OK;
}






//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK GameCodeApp::IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
                                      D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
   
    return true;
}



//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK GameCodeApp::OnD3D9LostDevice(void* pUserContext )
{
   
}

void CALLBACK GameCodeApp::OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext  )
{
	
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK GameCodeApp::OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext )
{
    

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK GameCodeApp::OnD3D9DestroyDevice( void* pUserContext )
{
	
}

