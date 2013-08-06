#pragma once

#include <DXUTgui.h>

class D3DRenderer : public IRenderer
{
public:
	static CDXUTDialogResourceManager g_DialogResourceManager;
	static CDXUTTextHelper* g_pTextHelper;

	virtual HRESULT VOnRestore() {return S_OK;}
	virtual void VShutDown() {SAFE_DELETE(g_pTextHelper);}
};

class D3DRenderer9 : public D3DRenderer
{
public:
	D3DRenderer9()
	{
		m_pFont = NULL;
		m_pTextSprite = NULL;
	}

	virtual void VSetBackgroundColor(BYTE bgA, BYTE bgR, BYTE bgG, BYTE bgB)
	{
		D3DCOLOR_ARGB(bgA, bgR, bgG, bgB);
	}

	virtual HRESULT VOnRestore();
	virtual void VShutDown()
	{
		D3DRenderer::VShutDown();
		SAFE_RELEASE(m_pFont);
		SAFE_RELEASE(m_pTextSprite);
	}

	virtual bool VPreRender();
	virtual bool VPostRender();
protected:
	D3DCOLOR m_backgroundColor;
	ID3DXFont* m_pFont;
	ID3DXSprite* m_pTextSprite;
};