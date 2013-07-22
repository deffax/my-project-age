#include "GameCodeStd.h"
#include "EventManager.h"


static IEventManager* g_pEventMgr = NULL;

GenericObjectFactory<IEventData, EventType> g_eventFactory;

IEventManager* IEventManager::Get()
{
	GCC_ASSERT(g_pEventMgr);
	return g_pEventMgr;
}

IEventManager::IEventManager(const char* pName, bool setAsGlobal)
{
	if(setAsGlobal)
	{
		if(g_pEventMgr)
		{
			GCC_ERROR("Attemping to create 2 globals managers.");
			delete g_pEventMgr;
		}
		g_pEventMgr = this;
	}
}

IEventManager::~IEventManager()
{
	if(g_pEventMgr == this)
		g_pEventMgr = NULL;
}
