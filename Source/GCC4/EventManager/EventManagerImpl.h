#include "EventManager.h"

const unsigned int EVENTMANAGER_NUM_QUEUES = 2;

class EventManager : public IEventManager
{
	typedef std::list<EventListenerDelegate> EventListenerList;
	typedef std::map<EventType, EventListenerList> EventListenerMap;
	typedef std::list<IEventDataPtr> EventQueue;

	EventListenerMap m_eventListeners;
	EventQueue m_queues[EVENTMANAGER_NUM_QUEUES];
	int m_activeQueue;

public:
	explicit EventManager(const char* pName, bool setAsGlobal);
	virtual ~EventManager();

	virtual bool VAddListener(const EventListenerDelegate& eventDelegate, const EventType& type);
	virtual bool VRemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type);

	virtual bool VTriggerEvent(const IEventDataPtr& pEvent) const;
	virtual bool VQueueEvent(const IEventDataPtr& pEvent);
	//virtual bool VThreadSafeQueueEvent(const IEventDataPtr& pEvent);
	virtual bool VAbortEvent(const IEventDataPtr& pEvent, bool allOfType = false) = 0;
	virtual bool VUpdate(unsigned long maxMillis = kINFINITE) = 0;
};