#include "GameCodeStd.h"
#include "../Mainloop/Initialization.h"
#include "../ResourceCache/ResCache.h"
#include "../ResourceCache/XmlResource.h"
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

	

	return true; //PROVVISORIO!!!!
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
