#pragma once
#include <Windows.h>

class CriticalSection: public GCC_noncopyable
{
public:
	CriticalSection()
	{
		InitializeCriticalSection(&m_cs);
	}

	~CriticalSection()
	{
		DeleteCriticalSection(&m_cs);
	}

	void Lock()
	{
		EnterCriticalSection(&m_cs);
	}

	void Unlock()
	{
		LeaveCriticalSection(&m_cs);
	}
protected:
	mutable CRITICAL_SECTION m_cs;
};