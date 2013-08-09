#include "GameCodeStd.h"
#include "D3DRenderer.h"

CDXUTDialogResourceManager D3DRenderer::g_DialogResourceManager;
CDXUTTextHelper *D3DRenderer::g_pTextHelper = NULL;


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

void D3DRenderer9::VDrawLine(const Vec3& from, const Vec3& to, const Color& color)
{
	DWORD oldLightingState;
	DXUTGetD3D9Device()->GetRenderState(D3DRS_LIGHTING, &oldLightingState);
	DXUTGetD3D9Device()->SetRenderState(D3DRS_LIGHTING, FALSE);
	D3D9Vertex_Colored verts[2];

	verts[0].position = from;
	verts[0].color = color;
	verts[1].position = to;
	verts[1].color = verts[0].color;

	DXUTGetD3D9Device()->SetFVF(D3D9Vertex_Colored::FVF);
	DXUTGetD3D9Device()->DrawPrimitiveUP(D3DPT_LINELIST, 1, verts, sizeof(D3D9Vertex_Colored::FVF));
	DXUTGetD3D9Device()->SetRenderState(D3DRS_LIGHTING, oldLightingState);
}