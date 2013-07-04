#pragma once

#pragma warning(disable: 4996)

class NWNCloneApp : public GameCodeApp
{
public:
	virtual TCHAR* VGetGameTitle(){return _T("NWN Clone");}
};