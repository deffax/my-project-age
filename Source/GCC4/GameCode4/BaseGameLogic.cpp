#include "GameCodeStd.h"
#include "../Mainloop/Process.h"
#include "../Actors/ActorFactory.h"
#include "../Actors/Actor.h"

#include "BaseGameLogic.h"

BaseGameLogic::BaseGameLogic()
{
	m_LastActorId = 0;
	m_LifeTime = 0;
	m_pProcessManager = GCC_NEW ProcessManager;
	m_pActorFactory = NULL;
	m_State = BGS_Initializing;
}

BaseGameLogic::~BaseGameLogic()
{
	while(! m_gameViews.empty())
		m_gameViews.pop_front();

	SAFE_DELETE(m_pProcessManager);
	SAFE_DELETE(m_pActorFactory);

	for(auto it = m_actors.begin(); it != m_actors.end(); ++it)
		it->second->Destroy();
	m_actors.clear();
}

void BaseGameLogic::VDestroyActor(const ActorId actorId)
{
	
}