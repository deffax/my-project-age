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