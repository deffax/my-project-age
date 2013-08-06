#pragma once

class HumanView : public IGameView
{
	friend class GameCodeApp;

protected:
	GameViewId m_ViewId;
	ActorId m_ActorId;
	//ProcessManager* m_pProcessManager;
	DWORD  m_currTick;
	DWORD m_lastDraw;
	bool m_runFullSpeed;


public:
	virtual HRESULT VOnRestore();
	virtual HRESULT VOnLostDevice();
	virtual void VOnRender(double fTime, float fElapsedTime);
	virtual GameViewType VGetType() const {return GameView_Human;}
	virtual GameViewId VGetId() const {return m_ViewId;}
	virtual void VOnAttach(GameViewId vid, ActorId aid)
	{
		m_ViewId = vid;
		m_ActorId = aid;
	}
	virtual void VOnUpdate(const int deltaMilliseconds);
	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);
	HumanView(shared_ptr<IRenderer> renderer);
	virtual ~HumanView();

	ScreenElementList m_ScreenElements;


	
};