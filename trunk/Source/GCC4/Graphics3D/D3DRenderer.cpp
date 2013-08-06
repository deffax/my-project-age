#include "GameCodeStd.h"
#include "D3DRenderer.h"


bool D3DRenderer9::VPreRender()
{
	if(SUCCEEDED(
		DXUTGetD3D9Device()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			m_backgroundColor, 1.0f, 0)
			))
	{
		return (SUCCEEDED(DXUTGetD3D9Device()->BeginScene()));
	}
	return false;
}

bool D3DRenderer9::VPostRender()
{
	return (SUCCEEDED(DXUTGetD3D9Device()->EndScene()));
}

HRESULT D3DRenderer9::VOnRestore()
{
	HRESULT hr;
	V_RETURN ( D3DRenderer::VOnRestore() );
	V_RETURN ( D3DRenderer::g_DialogResourceManager.OnD3D9ResetDevice() ); 

	SAFE_DELETE(D3DRenderer::g_pTextHelper);

	SAFE_RELEASE(m_pFont);
    V_RETURN(D3DXCreateFont( DXUTGetD3D9Device(), 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                    L"Arial", &m_pFont ) );

	SAFE_RELEASE(m_pTextSprite);
	V_RETURN( D3DXCreateSprite( DXUTGetD3D9Device(), &m_pTextSprite ) );
	
	D3DRenderer::g_pTextHelper = GCC_NEW CDXUTTextHelper( m_pFont, m_pTextSprite, 15 );			
	return S_OK;
}