#pragma once
#include "../Mainloop/Initialization.h"

class GameCodeApp
{
protected:
	std::map<std::wstring, std::wstring> m_textResource;
public:
	GameCodeApp();

	virtual TCHAR* VGetGameTitle() = 0;

	virtual bool InitInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd = NULL,
		int screenWidth = SCREEN_WIDTH, int screenHeight = SCREEN_HEIGHT);

	struct GameOptions m_Options;
	class ResCache* m_resCache;

	// DirectX9 Specific Stuff
	static bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
	static HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	static HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	static void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
	static void CALLBACK OnD3D9LostDevice( void* pUserContext );
	static void CALLBACK OnD3D9DestroyDevice( void* pUserContext );
};


extern GameCodeApp* g_pApp;