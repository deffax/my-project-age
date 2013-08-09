#pragma once

#pragma warning(disable: 4996)

class NWNCloneApp : public GameCodeApp
{
protected:
	virtual BaseGameLogic* VCreateGameAndView();
	virtual HICON VGetIcon();
public:
	virtual TCHAR* VGetGameTitle(){return _T("NWN Clone");}
};


class NWNCloneLogic: public BaseGameLogic
{
public:
	NWNCloneLogic(){};
	~NWNCloneLogic(){};
};