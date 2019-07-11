#pragma once
#include <vector>

class Object;

enum Event
{
	Collision,
	Bullet_Destroyed,
	Galaxian_Destroyed
};

class Observer
{
public:
	virtual void onEvent(Event event, Object *obj) = 0;
};

class EventManager
{
public:
	
	EventManager(std::vector<Object*> &objects);

	void pushEvent(Event event, Object *object);

	void addObserver(Observer *observer);

private:

	std::vector<Object*> &m_objects;
	std::vector<Observer*>m_observers;
};