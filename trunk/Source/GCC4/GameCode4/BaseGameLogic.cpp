#include "GameCodeStd.h"
#include "../Mainloop/Process.h"
#include "../Actors/ActorFactory.h"
#include "../Actors/Actor.h"
#include "../UserInterface/HumanView.h"

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

/*void BaseGameLogic::VDestroyActor(const ActorId actorId)
{
	
}*/

void BaseGameLogic::VChangeState(BaseGameState newState)
{
	m_State = newState;
}

void BaseGameLogic::VOnUpdate(float time, float elapsedTime)
{
	int deltaMilliseconds = int(elapsedTime * 1000.0f);
	m_LifeTime += elapsedTime;

	switch(m_State)
	{
	case BGS_Initializing:
		VChangeState(BGS_MainMenu);
		break;
	case BGS_MainMenu:		
		break;
	case BGS_LoadingGameEnvironment:
		break;
	case BGS_WaitingForPlayersToLoadEnvironment:
		break;
	case BGS_SpawningPlayersActors:
		VChangeState(BGS_Running);
		break;
	case BGS_WaitingForPlayers:
		break;
	case BGS_Running:
		break;
	default:
		GCC_ERROR("Unrecognized state.");
	}

	for(GameViewList::iterator it = m_gameViews.begin();
		it != m_gameViews.end(); ++it)
	{
		(*it)->VOnUpdate(deltaMilliseconds);
	}
}

