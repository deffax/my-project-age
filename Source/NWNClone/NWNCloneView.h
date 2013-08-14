#pragma once

#include <DXUTgui.h>
#include "../GCC4/UserInterface/HumanView.h"

class MainMenuUI : public BaseUI
{
protected:
	CDXUTDialog m_SampleUI;

public:
	MainMenuUI();
	virtual ~MainMenuUI();

	virtual HRESULT VOnRestore();
	virtual HRESULT VOnRender(double fTime, float fElapsedTime);
	virtual int VGetZOrder() const {return 1;}
	virtual void VSetZOrder(int const zOrder) {}
	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);

	static void CALLBACK OnGUIEvent(UINT nEvent, int nControlId,
		CDXUTControl* pControl, void* pUserContex);
	void CALLBACK _OnGUIEvent( UINT nEvent, int nControlID,
		CDXUTControl* pControl, void *pUserContext );
};


class MainMenuView : public HumanView
{
protected:
	shared_ptr<MainMenuUI> m_MainMenuUI;
public:
	MainMenuView();
	~MainMenuView();

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);
	virtual void VRenderText();
	virtual void VOnUpdate(unsigned long deltaMs);
};