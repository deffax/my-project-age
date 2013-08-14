#include "NWNCloneStd.h"
#include "NWNCloneView.h"
#include "../GCC4/Graphics3D/D3DRenderer.h"

MainMenuUI::MainMenuUI()
{
	m_SampleUI.Init(&D3DRenderer::g_DialogResourceManager);
	m_SampleUI.SetCallback(OnGUIEvent, this);


	D3DCOLOR color = 0x50505050;
	m_SampleUI.SetBackgroundColors(color);
}


MainMenuUI::~MainMenuUI()
{ 
	D3DRenderer::g_DialogResourceManager.UnregisterDialog(&m_SampleUI);
}

HRESULT MainMenuUI::VOnRestore()
{
	return S_OK;
}

HRESULT MainMenuUI::VOnRender(double fTime, float fElapsedTime)
{
	HRESULT hr;
	DXUT_BeginPerfEvent(DXUT_PERFEVENTCOLOR, L"NWNCloneHUD");
	V(m_SampleUI.OnRender(fElapsedTime));
	DXUT_EndPerfEvent();
	return S_OK;
}

LRESULT CALLBACK MainMenuUI::VOnMsgProc( AppMsg msg )
{
	return m_SampleUI.MsgProc( msg.m_hWnd, msg.m_uMsg, msg.m_wParam, msg.m_lParam );
}

void CALLBACK MainMenuUI::_OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void *pUserContext )
{}

void CALLBACK MainMenuUI::OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void *pUserContext )
{
	MainMenuUI *ui = static_cast<MainMenuUI *>(pUserContext);
	ui->_OnGUIEvent(nEvent, nControlID, pControl, pUserContext);
}



MainMenuView::MainMenuView() : HumanView(shared_ptr<IRenderer>())
{
	m_MainMenuUI.reset(GCC_NEW MainMenuUI);
	VPushElement(m_MainMenuUI);
}

MainMenuView::~MainMenuView()
{}

void MainMenuView::VRenderText()
{
	HumanView::VRenderText();
}

void MainMenuView::VOnUpdate(unsigned long deltaMs)
{
	HumanView::VOnUpdate(deltaMs);
}

LRESULT CALLBACK MainMenuView::VOnMsgProc(AppMsg msg)
{
	if(m_MainMenuUI->VIsVisible())
	{
		if(HumanView::VOnMsgProc(msg))
			return 1;
	}
	return 0;
}