

#include "GameCodeStd.h"
#include "ActorFactory.h"
#include "../ResourceCache/XmlResource.h"
#include "ActorComponent.h"
#include "Actor.h"

ActorFactory::ActorFactory(void)
{
    m_lastActorId = INVALID_ACTOR_ID;

   
}

StrongActorPtr ActorFactory::CreateActor(const char* actorResource, TiXmlElement *overrides, const ActorId serversActorId)
{
   
    TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(actorResource);
    if (!pRoot)
    {
        GCC_ERROR("Failed to create actor from resource: " + std::string(actorResource));
        return StrongActorPtr();
    }

    
	ActorId nextActorId = serversActorId;
	if (nextActorId == INVALID_ACTOR_ID)
	{
		nextActorId = GetNextActorId();
	}
    StrongActorPtr pActor(GCC_NEW Actor(nextActorId));
    if (!pActor->Init(pRoot))
    {
        GCC_ERROR("Failed to initialize actor: " + std::string(actorResource));
        return StrongActorPtr();
    }

	bool initialTransformSet = false;

    
    for (TiXmlElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
    {
        StrongActorComponentPtr pComponent(VCreateComponent(pNode));
        if (pComponent)
        {
            pActor->AddComponent(pComponent);
            pComponent->SetOwner(pActor);
        }
        else
        {
            
            return StrongActorPtr();
        }
    }

	if (overrides)
	{
		ModifyActor(pActor, overrides);
	}

	
    pActor->PostInit();

    return pActor;
}

StrongActorComponentPtr ActorFactory::VCreateComponent(TiXmlElement* pData)
{
    const char* name = pData->Value();
    StrongActorComponentPtr pComponent(m_componentFactory.Create(ActorComponent::GetIdFromName(name)));

    
    if (pComponent)
    {
        if (!pComponent->VInit(pData))
        {
            GCC_ERROR("Component failed to initialize: " + std::string(name));
            return StrongActorComponentPtr();
        }
    }
    else
    {
        GCC_ERROR("Couldn't find ActorComponent named " + std::string(name));
        return StrongActorComponentPtr();  // fail
    }

   
    return pComponent;
}


void ActorFactory::ModifyActor(StrongActorPtr pActor, TiXmlElement* overrides)
{
	
	for (TiXmlElement* pNode = overrides->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		ComponentId componentId = ActorComponent::GetIdFromName(pNode->Value());
		StrongActorComponentPtr pComponent = MakeStrongPtr(pActor->GetComponent<ActorComponent>(componentId));
		if (pComponent)
		{
			pComponent->VInit(pNode);

			

			pComponent->VOnChanged();		
		}
		else
		{
			pComponent = VCreateComponent(pNode);
			if (pComponent)
			{
				pActor->AddComponent(pComponent);
				pComponent->SetOwner(pActor);
			}
		}
	}		
}