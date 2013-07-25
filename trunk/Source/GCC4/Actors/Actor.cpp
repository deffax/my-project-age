#include "GameCodeStd.h"
#include "Actor.h"
#include "ActorComponent.h"

Actor::Actor(ActorId id)
{
	m_id = id;
	m_type = "Unknown";
}

Actor::~Actor()
{
	GCC_LOG("Actor", "Destroying Actor " + ToStr(m_id));
	GCC_ASSERT(m_components.empty());
}

bool Actor::Init(TiXmlElement* pData)
{
	GCC_LOG("Actor", "Initializing Actor " + ToStr(m_id));
	m_type = pData->Attribute("type");
	return true;
}

void Actor::PostInit()
{
	for(ActorComponents::iterator it = m_components.begin();
		it != m_components.end(); ++it)
	{
		it->second->VPostInit();
	}
}

void Actor::Destroy()
{
	m_components.clear();
}

void Actor::Update(int deltaMs)
{
	for(ActorComponents::iterator it = m_components.begin();
		it != m_components.end(); ++it)
	{
		it->second->VUpdate(deltaMs);
	}
}

void Actor::AddComponent(StrongActorComponentPtr pComponent)
{
	std::pair<ActorComponents::iterator, bool> success = 
		m_components.insert(std::make_pair(pComponent->VGetId(), pComponent));
	GCC_ASSERT(success.second);
}