#pragma once

#include <DXUTgui.h>

class MessageBox : public BaseUI
{
protected:
	CDXUTDialog m_UI;
	int m_ButtonId;
public:
	MessageBox(std::wstring msg, std::wstring title, int buttonFlags = MB_OK);
	~MessageBox();

	virtual HRESULT VOnRestore();
	virtual HRESULT VOnRender(double fTime, float fElapsedTime);
	virtual int VGetZOrder() const {return 99;}
	virtual void VSetZOrder(int const zOrder){}
	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);

	static void CALLBACK OnGuiEvent(UINT nEvent, int nControlId,
		CDXUTControl* pControl, void* pUserContex);
	static int Ask(MessageBox_Questions question);

};