#include "GameCodeStd.h"

#include "../Mainloop/Initialization.h"
#include "../Graphics3D/D3DRenderer.h"
#include "../EventManager/EventManagerImpl.h"
#include "../LUAScripting/LuaStateManager.h"
#include "../ResourceCache/ResCache.h"
#include "../UserInterface/UserInterface.h"
#include "../ResourceCache/XmlResource.h"
#include "../UserInterface/HumanView.h"
#include "../Utilities/String.h"


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

	m_hInstance = hInstance;

	IResourceFile *zipFile = GCC_NEW ResourceZipFile(L"Assets.zip");
	m_resCache = GCC_NEW ResCache(50, zipFile);

	if (!m_resCache->Init())
	{
        GCC_ERROR("Failed to initialize resource cache!  Are your paths set up correctly?");
		return false;
	}
	
	

	extern shared_ptr<IResourceLoader> CreateXmlResourceLoader();

	m_resCache->RegisterLoader(CreateXmlResourceLoader());

	if(!LoadStrings("English"))
	{
		GCC_ERROR("Failed to Load strings");
		return false;
	}

	if(!LuaStateManager::Create())
		{
		GCC_ERROR("Failed to create LuaStateManager");
		return false;
	}

	m_pEventManager = GCC_NEW EventManager("GameCodeApp Event Mgr", true);
	if(!m_pEventManager)
	{
		GCC_ERROR("Failed to create EventManager");
		return false;
	}

	DXUTInit(true, true, lpCmdLine, true);
	if(hWnd == NULL)
	{
		DXUTCreateWindow(VGetGameTitle(), hInstance, VGetIcon());
	}
	else
	{
		DXUTSetWindow(hWnd, hWnd, hWnd);
	}
	if(!GetHwnd())
		return FALSE;

	SetWindowText(GetHwnd(), VGetGameTitle());

	m_screenSize = Point(screenWidth, screenHeight);
	DXUTCreateDevice( D3D_FEATURE_LEVEL_10_1, true, screenWidth, screenHeight);
	if (GetRendererImpl() == Renderer_D3D9)
	{
		m_Renderer = shared_ptr<IRenderer>(GCC_NEW D3DRenderer9());
	}
	else if (GetRendererImpl() == Renderer_D3D11)
	{
		//m_Renderer = shared_ptr<IRenderer>(GCC_NEW D3DRenderer11());
	}
	m_Renderer->VSetBackgroundColor(255, 20, 20, 200);
	//m_Renderer->VOnRestore();
	m_pGame = VCreateGameAndView();
	if(!m_pGame)
		return false;

	m_bIsRunning = true;
	return true;
}


std::wstring GameCodeApp::GetString(std::wstring sID)
{
	auto localizedString = m_textResource.find(sID);
	if(localizedString == m_textResource.end())
	{
		GCC_ASSERT(0 && "String not found");
		return L"";
	}
	return localizedString->second;
}

UINT GameCodeApp::MapCharToKeyCode(const char pHotKey)
{
	if(pHotKey >= '0' && pHotKey <= '9')
		return 0x30 + pHotKey -'0';

	if(pHotKey >= 'A' && pHotKey <= 'Z')
		return 0x41 + pHotKey -'A';

	GCC_ASSERT(0 && "Platform specific hotkey is not defined");
	return 0;
}

bool GameCodeApp::LoadStrings(std::string language)
{
	std::string languageFile = "Strings\\";
	languageFile += language;
	languageFile += ".xml";

	TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(languageFile.c_str());
	if(!pRoot)
	{
		GCC_ERROR("Strings are missing.");
		return false;
	}

	for(TiXmlElement* pElem = pRoot->FirstChildElement();
		pElem; pElem = pElem->NextSiblingElement())
	{
		const char* pKey = pElem->Attribute("id");
		const char* pText = pElem->Attribute("value");
		const char* pHotKey = pElem->Attribute("hotkey");

		if(pKey && pText)
		{
			wchar_t wideKey[64];
			wchar_t wideText[1024];
			AnsiToWideCch(wideKey, pKey, 64);
			AnsiToWideCch(wideText, pText, 1024);
			
			m_textResource[std::wstring(wideKey)] = std::wstring(wideText);

			if(pHotKey)
			{
				m_hotkeys[std::wstring(wideKey)] = MapCharToKeyCode(*pHotKey);
			}
		}
	}
	return true;
}

LRESULT GameCodeApp::OnNcCreate(LPCREATESTRUCT cs)
{
	return true;
}

LRESULT GameCodeApp::OnDisplayChange(int colorDepth, int width, int height)
{
	m_rcDesktop.left = 0;
	m_rcDesktop.top = 0;
	m_rcDesktop.right = width;
	m_rcDesktop.left = height;
	m_iColorDepth = colorDepth;
	return 0;
}


LRESULT GameCodeApp::OnPowerBroadCast(int event)
{
	if(event == PBT_APMQUERYSUSPEND)
		return BROADCAST_QUERY_DENY;
	else if(event == PBT_APMBATTERYLOW)
	{
		AbortGame();
	}
	return true;
}

LRESULT GameCodeApp::OnClose()
{
	SAFE_DELETE(m_pGame);
	DestroyWindow(GetHwnd());
	SAFE_DELETE(m_pEventManager);
	SAFE_DELETE(m_resCache);
	return 0;
}

LRESULT GameCodeApp::OnAltEnter()
{
	DXUTToggleFullScreen();
	return 0;
}

/*
LRESULT GameCodeApp::OnSysCommand(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case SC_MAXIMIZE :
		{
			
			if ( m_bWindowedMode && IsRunning() )
			{				
				OnAltEnter();
			}
		}
		return 0;

		case SC_CLOSE :
		{			
			if ( lParam != g_QuitNoPrompt )
			{				
				if ( m_bQuitRequested )
					return true;
								
				m_bQuitRequested = true;
				
				if ( MessageBox::Ask(QUESTION_QUIT_GAME) == IDNO )
				{					
					m_bQuitRequested = false;
					
					return true;
				}
			}
			m_bQuitting = true;
			
			if ( HasModalDialog() )
			{				
				ForceModalExit();				
				PostMessage( GetHwnd(), WM_SYSCOMMAND, SC_CLOSE, g_QuitNoPrompt );
				m_bQuitRequested = false;		
				return true;
			}			
			m_bQuitRequested = false;
		}
		return 0;
		default:			
			return DefWindowProc(GetHwnd(), WM_SYSCOMMAND, wParam, lParam);
	}
	return 0;
}

*/


HWND GameCodeApp::GetHwnd()
{ 
	return DXUTGetHWND();
} 




HumanView* GameCodeApp::GetHumanView()
{
	HumanView* pView = NULL;
	for(GameViewList::iterator i = m_pGame->m_gameViews.begin();
		i != m_pGame->m_gameViews.end(); ++i)
	{
		if((*i)->VGetType() == GameView_Human)
		{
			shared_ptr<IGameView> pIGameView(*i);
			pView = static_cast<HumanView* >(&*pIGameView);
			break;
		}
	}
	return pView;
}

void GameCodeApp::FlashWhileMinimized()
{
	if(!GetHwnd())
		return;
	if(IsIconic(GetHwnd()))
	{
		DWORD now = timeGetTime();
		DWORD then = now;
		MSG msg;
		FlashWindow(GetHwnd(), true);

		while(true)
		{
			if ( PeekMessage( &msg, NULL, 0, 0, 0 ) )
			{
				if ( msg.message != WM_SYSCOMMAND || msg.wParam != SC_CLOSE )
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				
				// Are we done?
				if ( ! IsIconic(GetHwnd()) )
				{
					FlashWindow( GetHwnd(), false );
					break;
				}
			}
			else
			{
				now = timeGetTime();
				DWORD timeSpan = now > then ? (now - then) : (then - now);
				if ( timeSpan > 1000 )
				{
					then = now;
					FlashWindow( GetHwnd(), true );
				}
			}
		}
	}
}

int GameCodeApp::PumpUntilMessage (UINT msgEnd, WPARAM* pWParam, LPARAM* pLParam)
{
	int currentTime = timeGetTime();
	MSG msg;
	for ( ;; )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if (msg.message == WM_CLOSE)
			{
				m_bQuitting = true;
				GetMessage(& msg, NULL, 0, 0);
				break;
			}
			else
			{
				// Default processing
				if (GetMessage(&msg, NULL, NULL, NULL))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				// Are we done?
				if ( msg.message == msgEnd)
					break;
			}
		}
		else
		{
			// Update the game views, but nothing else!
			// Remember this is a modal screen.
			if (m_pGame)
			{
				int timeNow = timeGetTime();
				int deltaMilliseconds = timeNow - currentTime;
				for(GameViewList::iterator i=m_pGame->m_gameViews.begin(); i!=m_pGame->m_gameViews.end(); ++i)
				{
					(*i)->VOnUpdate( deltaMilliseconds );
				}
				currentTime = timeNow;
				DXUTRender3DEnvironment();
			}
		}
	}
	if (pLParam)
		*pLParam = msg.lParam;
	if (pWParam)
		*pWParam = msg.wParam;

	return 0;
}

int GameCodeApp::Modal(shared_ptr<IScreenElement> pModalScreen, int defaultAnswer)
{
	return 1; // PROVVISORIO!
}


GameCodeApp::Renderer GameCodeApp::GetRendererImpl()
{
	if(DXUTGetDeviceSettings().ver == DXUT_D3D9_DEVICE)
		return Renderer_D3D9;
	else
		return Renderer_D3D11;

	return Renderer_Unknown;
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

bool CALLBACK GameCodeApp::IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
                                       DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	return true;
}

HRESULT CALLBACK GameCodeApp::OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                     void* pUserContext )
{
	return S_OK;
}


HRESULT CALLBACK GameCodeApp::OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                         const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{  

    return S_OK;
}



void CALLBACK GameCodeApp::OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
                                 float fElapsedTime, void* pUserContext )
{
	
}



void CALLBACK GameCodeApp::OnD3D11ReleasingSwapChain( void* pUserContext )
{
    
}



void CALLBACK GameCodeApp::OnD3D11DestroyDevice( void* pUserContext )
{
    
}


