#pragma once
#include "../Utilities/String.h"
class TiXmlElement;

class ActorComponent
{
	friend class ActorFactory;
protected:
	StrongActorPtr m_pOwner;

public:
	virtual ~ActorComponent(){m_pOwner.reset();}

	virtual bool VInit(TiXmlElement* pData) = 0;
	virtual void VPostInit(){}
	virtual void VUpdate(int deltaMs){}
	virtual void VOnChanged(){}

	virtual ComponentId VGetId() const {return GetIdFromName(VGetName());}
	virtual const char* VGetName() const = 0;
	static ComponentId GetIdFromName(const char* componentStr)
	{
		void* rawId = HashedString::hash_name(componentStr);
		return reinterpret_cast<ComponentId>(rawId);
	}

private:
	void SetOwner(StrongActorPtr pOwner) {m_pOwner = pOwner;}
};