#pragma once

extern bool IsOnlyInstance(LPCTSTR gameTitle);
extern bool CheckStorage(const DWORDLONG diskSpaceNeed);
extern DWORD ReadCPUSpeed();

struct GameOptions
{

	//rendering options
	std::string m_Renderer;
	bool m_runFullSpeed;
	Point m_ScreenSize;


    // resource cache options
    bool m_useDevelopmentDirectories;

	// TiXmlElement - look at this to find other options added by the developer
	TiXmlDocument *m_pDoc;

	GameOptions();
	~GameOptions() { SAFE_DELETE(m_pDoc); }

	void Init(const char* xmlFilePath, LPWSTR lpCmdLine);
};