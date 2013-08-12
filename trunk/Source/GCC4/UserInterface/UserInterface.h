#pragma once 


const DWORD g_QuitNoPrompt = MAKELPARAM(-1,-1);
const UINT g_MsgEndModal = (WM_USER+100);

enum MessageBox_Questions {
	QUESTION_WHERES_THE_CD,
	QUESTION_QUIT_GAME,
};

class BaseUI : public IScreenElement
{
protected:
	int m_PosX, m_PosY;
	int m_Width, m_Height;
	optional<int> m_Result;
	bool m_bIsVisible;
public:
	BaseUI()
	{
		m_bIsVisible = true;
		m_PosX = m_PosY = 0;
		m_Width = m_Height = 100;
	}
	virtual void VOnUpdate(int) {}
	virtual HRESULT VOnLostDevice() {return S_OK;}
	virtual bool VIsVisible() const {return m_bIsVisible;}
	virtual void VSetVisible(bool visible) {m_bIsVisible = visible;}
};