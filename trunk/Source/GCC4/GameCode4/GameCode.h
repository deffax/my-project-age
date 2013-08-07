#pragma once
#include "../Mainloop/Initialization.h"

class EventManager;

class GameCodeApp
{
protected:
	Point m_screenSize;

	std::map<std::wstring, std::wstring> m_textResource;
	std::map<std::wstring, UINT> m_hotkeys;
public:
	GameCodeApp();
	
	const Point &GetScreenSize() {return m_screenSize;}

	virtual TCHAR* VGetGameTitle() = 0;

	virtual bool InitInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd = NULL,
		int screenWidth = SCREEN_WIDTH, int screenHeight = SCREEN_HEIGHT);

	enum Renderer
	{
		Renderer_Unknown,
		Renderer_D3D9,
		Renderer_D3D11
	};

	shared_ptr<IRenderer> m_Renderer;

	static Renderer GetRendererImpl();

	struct GameOptions m_Options;
	class ResCache* m_resCache;

	EventManager* m_pEventManager;

	bool LoadStrings(std::string language);
	std::wstring GetString(std::wstring sID);
	UINT MapCharToKeyCode(const char pHotKey);

	// DirectX9 Specific Stuff
	static bool CALLBACK IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
	static HRESULT CALLBACK OnD3D9CreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	static HRESULT CALLBACK OnD3D9ResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	static void CALLBACK OnD3D9FrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
	static void CALLBACK OnD3D9LostDevice( void* pUserContext );
	static void CALLBACK OnD3D9DestroyDevice( void* pUserContext );

// DirectX 11 Specific Stuff
	static bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
	static HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	static HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	static void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext );
	static void CALLBACK OnD3D11DestroyDevice( void* pUserContext );
	static void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext );


};


extern GameCodeApp* g_pApp;