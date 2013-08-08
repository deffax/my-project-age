#pragma once

#include "../Mainloop/ProcessManager.h"
#include "../EventManager/EventManager.h"
#include "../Actors/Actor.h"

class ActorFactory;

enum BaseGameState
{
	BGS_Invalid,
	BGS_Initializing,
	BGS_MainMenu,
	BGS_WaitingForPlayers,
	BGS_LoadingGameEnvironment,
	BGS_WaitingForPlayersToLoadEnvironment,
	BGS_SpawningPlayersActors,
	BGS_Running
};

typedef std::map<ActorId, StrongActorPtr> ActorMap;

class BaseGameLogic : public IGameLogic
{
	friend class GameCodeApp;
protected:
	float m_LifeTime;
	ProcessManager* m_pProcessManager;
	ActorMap m_actors;
	ActorId m_LastActorId;
	BaseGameState m_State;
	GameViewList m_gameViews;
	ActorFactory* m_pActorFactory;

public:
	BaseGameLogic();
	~BaseGameLogic();


	virtual WeakActorPtr VGetActor(const ActorId id);
	virtual StrongActorPtr VCreateActor(const std::string& actorResource, TiXmlElement* overrides,
		Mat4x4* initialTransform);
	virtual void VDestroyActor(const ActorId actorId);
	virtual void VOnUpdate(float time, float elapsedTime);
	virtual void VChangeState(enum BaseGameState newState);
};