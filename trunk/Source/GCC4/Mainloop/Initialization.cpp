#include "GameCodeStd.h"
#include <direct.h>
#include "Initialization.h"






bool IsOnlyInstance(LPCTSTR gameTitle)
{
	HANDLE handle = CreateMutex(NULL,TRUE, gameTitle);
	if(GetLastError() != ERROR_SUCCESS)
	{
		HWND hWnd = FindWindow(gameTitle, NULL);
		if(hWnd)
		{
			ShowWindow(hWnd, SW_SHOWNORMAL);
			SetFocus(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);
			return false;
		}
	}
	return true;
}


bool CheckStorage(const DWORDLONG diskSpaceNeeded)
{
	int const drive = _getdrive();
	struct _diskfree_t diskfree;
	_getdiskfree(drive, &diskfree);

	unsigned __int64 const needClusters = diskSpaceNeeded / (diskfree.sectors_per_cluster *
		diskfree.bytes_per_sector);

	if(diskfree.avail_clusters < needClusters)
	{
		GCC_ERROR("CheckStorage Failure: Not enough physical storage.");
		return false;
	}
	return true;
}


DWORD ReadCPUSpeed()
{
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz = 0;
	DWORD type = REG_DWORD;
	HKEY hKey;
	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
		0, KEY_READ, &hKey);
	if(lError == ERROR_SUCCESS)
	{
		RegQueryValueEx(hKey, L"~MHz", NULL, &type, (LPBYTE) &dwMHz, &BufSize);
	}
	return dwMHz;
}

GameOptions::GameOptions()
{
	m_useDevelopmentDirectories = false;
	m_pDoc = NULL;
}

void GameOptions::Init(const char* xmlFileName, LPWSTR lpCmdLine)
{
	

	m_pDoc = new TiXmlDocument(xmlFileName);
	if (m_pDoc && m_pDoc->LoadFile())
	{
        TiXmlElement *pRoot = m_pDoc->RootElement();
        if (!pRoot)
            return;

        TiXmlElement* pNode = NULL;

		pNode = pRoot->FirstChildElement("ResCache");
		if(pNode)
		{
			std::string attribute(pNode->Attribute("useDevelopmentDirectories"));
			m_useDevelopmentDirectories = ((attribute == "yes") ? (true) : (false));
		}
	}
}