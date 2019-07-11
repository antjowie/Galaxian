#include "EventManager.h"
#include "objects/Object.h"

EventManager::EventManager(std::vector<Object*>& objects) :
	m_objects(objects)
{
}

void EventManager::pushEvent(Event event, Object * obj)
{
	for (std::vector<Object*>::iterator object = m_objects.begin();
		object != m_objects.end(); object++)
		(*object)->onEvent(event, obj);

	for (std::vector<Observer*>::iterator observer = m_observers.begin();
		observer != m_observers.end(); observer++)
		(*observer)->onEvent(event, obj);
}

void EventManager::addObserver(Observer * observer)
{
	m_observers.push_back(observer);
}
