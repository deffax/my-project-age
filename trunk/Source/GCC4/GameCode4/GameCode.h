#pragma once
#include "../Mainloop/Initialization.h"
#include "BaseGameLogic.h"

#include "../UserInterface/UserInterface.h"

class EventManager;

class HumanView;

class GameCodeApp
{
protected:
	Point m_screenSize;
	HINSTANCE m_hInstance;
	bool m_bIsRunning;
	bool m_bQuitting;
	Rect m_rcDesktop;
	int m_iColorDepth;
	int m_HasModalDialog;
	bool m_bWindowedMode;
	bool m_bQuitRequested;

	std::map<std::wstring, std::wstring> m_textResource;
	std::map<std::wstring, UINT> m_hotkeys;

	
	int PumpUntilMessage (UINT msgEnd, WPARAM* pWParam, LPARAM* pLParam);
	void FlashWhileMinimized();
public:
	GameCodeApp();
	
	const Point &GetScreenSize() {return m_screenSize;}

	virtual TCHAR *VGetGameTitle()=0;
	//virtual TCHAR *VGetGameAppDirectory()=0;
	virtual HICON VGetIcon()=0;
	
	LRESULT OnDisplayChange(int colorDepth, int width, int height);
	LRESULT OnNcCreate(LPCREATESTRUCT cs);
	LRESULT OnPowerBroadCast(int event);
	LRESULT OnClose();
	LRESULT OnAltEnter();
	LRESULT OnSysCommand(WPARAM wParam, LPARAM lParam);

	
	int Modal(shared_ptr<IScreenElement> pModalScreen, int defaultAnswer);
	bool HasModalDialog() { return m_HasModalDialog!=0; }
	void ForceModalExit() { PostMessage(GetHwnd(), g_MsgEndModal, 0, g_QuitNoPrompt);}

	HWND GetHwnd();
	HINSTANCE GetInstance() { return m_hInstance; }
	virtual bool InitInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd = NULL,
		int screenWidth = SCREEN_WIDTH, int screenHeight = SCREEN_HEIGHT);

	
	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
		bool* pNoFurtherProcessing, void* pUserContext);

	enum Renderer
	{
		Renderer_Unknown,
		Renderer_D3D9,
		Renderer_D3D11
	};

	shared_ptr<IRenderer> m_Renderer;

	static Renderer GetRendererImpl();
	BaseGameLogic* m_pGame;
	struct GameOptions m_Options;
	class ResCache* m_resCache;

	EventManager* m_pEventManager;
	BaseGameLogic* GetGameLogic(void) const { return m_pGame; }
	bool LoadStrings(std::string language);
	std::wstring GetString(std::wstring sID);
	UINT MapCharToKeyCode(const char pHotKey);

	HumanView* GetHumanView();
	virtual BaseGameLogic *VCreateGameAndView()=0;
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

	
	void AbortGame() { m_bQuitting = true; }
	int GetExitCode() { return DXUTGetExitCode(); }
	bool IsRunning() { return m_bIsRunning; }
	void SetQuitting(bool quitting) { m_bQuitting = quitting; }

};


extern GameCodeApp* g_pApp;