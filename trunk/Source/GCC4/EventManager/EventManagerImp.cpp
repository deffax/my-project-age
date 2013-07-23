#include "GameCodeStd.h"
#include "EventManagerImpl.h"
#include "../Utilities/String.h"

EventManager::EventManager(const char* pName, bool setAsGlobal)
	: IEventManager(pName, setAsGlobal)
{
	m_activeQueue = 0;
}

EventManager::~EventManager()
{

}

bool EventManager::VAddListener(const EventListenerDelegate& eventDelegate, const EventType& type)
{
	GCC_LOG("Events", "Attemping to add delegate function for event type: " + ToStr(type, 16));
	EventListenerList& eventListenerList = m_eventListeners[type];
	for(auto it = eventListenerList.begin(); it != eventListenerList.end(); ++it)
	{
		if(eventDelegate == (*it))
		{
			GCC_WARNING("Attemping to double register a delegate");
			return false;
		}
	}
	eventListenerList.push_back(eventDelegate);
	GCC_LOG("Events", "Successfully added delegate for event type: " + ToStr(type, 16));

	return true;
}

bool EventManager::VRemoveListener(const EventListenerDelegate& eventDelegate, const EventType& type)
{
	GCC_LOG("Events", "Attemping to remove delegate function for event type: " + ToStr(type, 16));

	bool success = false;

	auto findIt = m_eventListeners.find(type);
	if(findIt != m_eventListeners.end())
	{
		EventListenerList& listeners = findIt->second;
		for(auto it = listeners.begin(); it != listeners.end(); ++it)
		{
			if(eventDelegate == (*it))
			{
				listeners.erase(it);
				GCC_LOG("Events", "Successfully removed delegate for event type: " + ToStr(type, 16));
				success = true;
				break;
			}
		}
	}
	return success;
}

bool EventManager::VTriggerEvent(const IEventDataPtr& pEvent) const
{
	GCC_LOG("Events", "Attemping to trigger event " + std::string(pEvent->GetName()));

	bool processed = false;

	auto findIt = m_eventListeners.find(pEvent->VGetEventType());
	if(findIt != m_eventListeners.end())
	{
		const EventListenerList& eventListenerList = findIt->second;
		for(EventListenerList::const_iterator it =
			eventListenerList.begin(); it != eventListenerList.end(); ++it)
		{
			EventListenerDelegate listener = (*it);
			GCC_LOG("Events", "Sending event " + std::string(pEvent->GetName()) + " to delegate.");
			listener(pEvent);
			processed = true;
		}
	}
	return processed;
}

bool EventManager::VQueueEvent(const IEventDataPtr& pEvent)
{
	GCC_ASSERT(m_activeQueue >= 0);
	GCC_ASSERT(m_activeQueue < EVENTMANAGER_NUM_QUEUES);

	if(!pEvent)
	{
		GCC_ERROR("Invalid event in VQueueEvent.");
		return false;
	}

	GCC_LOG("Events", "Attemping to queue event " + std::string(pEvent->GetName()));

	auto findIt = m_eventListeners.find(pEvent->VGetEventType());
	if(findIt != m_eventListeners.end())
	{
		m_queues[m_activeQueue].push_back(pEvent);
		GCC_LOG("Events", "Successfully queued event " + std::string(pEvent->GetName()));
		return true;
	}
	else
	{
		GCC_LOG("Events", "Skipping event, since there are no delegate registered to recive it: "
			+ std::string(pEvent->GetName()));
		return false;
	}
}

bool EventManager::VAbortEvent(const EventType& inType, bool allOfType)
{
	GCC_ASSERT(m_activeQueue >= 0);
	GCC_ASSERT(m_activeQueue < EVENTMANAGER_NUM_QUEUES);

	bool success = false;
	EventListenerMap::iterator findIt = m_eventListeners.find(inType);
	if(findIt != m_eventListeners.end())
	{
		EventQueue& eventQueue = m_queues[m_activeQueue];
		auto it = eventQueue.begin();
		while(it != eventQueue.end())
		{
			auto thisIt = it;
			++it;
			if((*thisIt)->VGetEventType() == inType)
			{
				eventQueue.erase(thisIt);
				success = true;
				if(!allOfType)
					break;
			}
		}
	}
	return success;
}

bool EventManager::VUpdate(unsigned long maxMillis)
{
	unsigned long currMs = GetTickCount();
	unsigned long maxMs = ((maxMillis == IEventManager::kINFINITE) ? (IEventManager::kINFINITE) : (currMs + maxMillis));

	

    int queueToProcess = m_activeQueue;
	m_activeQueue = (m_activeQueue + 1) % EVENTMANAGER_NUM_QUEUES;
	m_queues[m_activeQueue].clear();

    GCC_LOG("EventLoop", "Processing Event Queue " + ToStr(queueToProcess) + "; " + ToStr((unsigned long)m_queues[queueToProcess].size()) + " events to process");

	
	while (!m_queues[queueToProcess].empty())
	{
        
		IEventDataPtr pEvent = m_queues[queueToProcess].front();
        m_queues[queueToProcess].pop_front();
        GCC_LOG("EventLoop", "\t\tProcessing Event " + std::string(pEvent->GetName()));

		const EventType& eventType = pEvent->VGetEventType();

       
		auto findIt = m_eventListeners.find(eventType);
		if (findIt != m_eventListeners.end())
		{
			const EventListenerList& eventListeners = findIt->second;
            GCC_LOG("EventLoop", "\t\tFound " + ToStr((unsigned long)eventListeners.size()) + " delegates");

           
			for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it)
			{
                EventListenerDelegate listener = (*it);
                GCC_LOG("EventLoop", "\t\tSending event " + std::string(pEvent->GetName()) + " to delegate");
				listener(pEvent);
			}
		}

        
		currMs = GetTickCount();
		if (maxMillis != IEventManager::kINFINITE && currMs >= maxMs)
        {
            GCC_LOG("EventLoop", "Aborting event processing; time ran out");
			break;
        }
	}
	
	
	bool queueFlushed = (m_queues[queueToProcess].empty());
	if (!queueFlushed)
	{
		while (!m_queues[queueToProcess].empty())
		{
			IEventDataPtr pEvent = m_queues[queueToProcess].back();
			m_queues[queueToProcess].pop_back();
			m_queues[m_activeQueue].push_front(pEvent);
		}
	}
	
	return queueFlushed;
}

