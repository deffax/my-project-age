#pragma once

class TiXmlElement;
typedef std::string ActorType;

class Actor
{
	friend class ActorFactory;
public:
	typedef std::map<ComponentId, StrongActorComponentPtr> ActorComponents;
private:
	ActorId m_id;
	ActorComponents m_components;
	ActorType m_type;
public:

	explicit Actor(ActorId id);
	~Actor();

	bool Init(TiXmlElement* pData);
	void PostInit();

	void Destroy();
	void Update(int deltaMs);

	ActorId GetId() const {return m_id;}
	ActorType GetType() const {return m_type;}

	template<class ComponentType>
	weak_ptr<ComponentType> GetComponent(ComponentId id)
	{
		ActorComponents::iterator findIt = m_components.find(id);
		if(findIt != m_components.end())
		{
			StrongActorComponentPtr pBase(findIt->second);
			shared_ptr<ComponentType> pSub(static_pointer_cast<ComponentType>(pBase));
			weak_ptr<ComponentType> pWeakSub(pSub);
			return pWeakSub;
		}
		else
		{
			return weak_ptr<ComponentType>();
		}
	}

	template<class ComponentType>
	weak_ptr<ComponentType> GetComponent(const char* name)
	{
		ComponentId id = ActorComponent::GetIdFromName(name);
		ActorComponents::iterator findIt = m_components.find(id);
		if(findIt != m_components.end())
		{
			StrongActorComponentPtr pBase(findIt->second);
			shared_ptr<ComponentType> pSub(static_pointer_cast<ComponentType>(pBase));
			weak_ptr<ComponentType> pWeakSub(pSub);
			return pWeakSub;
		}
		else
		{
			return weak_ptr<ComponentType>();
		}
	}

	const ActorComponents* GetComponents() {return &m_components;}
	void AddComponent(StrongActorComponentPtr pComponent);


};