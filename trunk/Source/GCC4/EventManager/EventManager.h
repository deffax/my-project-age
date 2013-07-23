#pragma once
#include <strstream>
#include "../3rdParty/FastDelegate/FastDelegate.h"

class IEventData;

typedef unsigned long EventType;
typedef shared_ptr<IEventData> IEventDataPtr;
typedef fastdelegate::FastDelegate1<IEventDataPtr> EventListenerDelegate;


extern GenericObjectFactory<IEventData, EventType> g_eventFactory;
#define REGISTER_EVENT(eventClass) g_eventFactory.Register<eventClass>(eventClass::sk_EventType);
#define CREATE_EVENT(eventType) g_eventFactory.Create(eventType);

class IEventData
{
public:
	virtual ~IEventData(){}
	virtual const EventType& VGetEventType() const = 0;
	virtual float GetTimeStamp() const = 0;
	virtual void VSerialize(std::ostrstream& out) const = 0;
	virtual void VDeserialize(std::istrstream& in) = 0;
	virtual IEventDataPtr VCopy() const = 0;
	virtual const char* GetName() const = 0;
};

class BaseEventData : public IEventData
{
	const float m_timeStamp;
public:
	explicit BaseEventData(const float timeStamp = 0.0f)
		: m_timeStamp(timeStamp) {}

	virtual const EventType& VGetEventType() const = 0;
	virtual void VSerialize(std::ostrstream& out) const {}
	virtual void VDeserialize(std::istrstream& in) {}
	float GetTimeStamp() const {return m_timeStamp;}
};

class IEventManager
{
public:
	enum eConstants {kINFINITE = 0xffffffff};

	explicit IEventManager(const char* pName, bool setAsGlobal);
	virtual ~IEventManager();

	virtual bool VAddListener(const EventListenerDelegate& eventDelegate, const EventType& type) = 0;
	virtual bool VRemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type) = 0;

	virtual bool VTriggerEvent(const IEventDataPtr& pEvent) const = 0;
	virtual bool VQueueEvent(const IEventDataPtr& pEvent) = 0;
	//virtual bool VThreadSafeQueueEvent(const IEventDataPtr& pEvent) = 0;
	virtual bool VAbortEvent(const EventType& pEvent, bool allOfType = false) = 0;
	virtual bool VUpdate(unsigned long maxMillis = kINFINITE) = 0;

	static IEventManager* Get(void);
};