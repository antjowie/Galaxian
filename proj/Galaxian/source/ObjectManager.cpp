#include "ObjectManager.h"
#include "objects/Object.h"
#include "Game.h"
#include <algorithm>

Object* ObjectManager::push(Object * object)
{
	m_objectsToAdd.push_back(object);
	return object;
}

void ObjectManager::update(const float elapsedTime)
{
	// Update all objects
	for (std::vector<Object*>::iterator object = m_objects.begin(); object != m_objects.end(); object++)
		if(!(*object)->m_dead)
			(*object)->update(elapsedTime);

	// Remove dead objects
	m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [&](Object *object)
		{bool dead = object->m_dead; if (dead) delete object; return dead; }), m_objects.end());
	
	// Add new objects to vector if objects needed to be added
	// Adding objects while updating resulted into a crash
	m_objects.insert(m_objects.end(), m_objectsToAdd.begin(), m_objectsToAdd.end());
	m_objectsToAdd.clear();
}

void ObjectManager::clear()
{
	m_objects.clear();
}

void ObjectManager::draw(Game & app) const
{
	for (std::vector<Object*>::const_iterator object = m_objects.begin(); object != m_objects.end(); object++)
		app.DrawSprite((*object)->m_id);
}

std::vector<Object*>& ObjectManager::getObjects()
{
	return m_objects;
}

Object* ObjectManager::getObjectById(const unsigned int id) const
{
	for (size_t i = 0; i < m_objects.size(); i++)
		if (m_objects[i]->m_id == id)
			return m_objects[i];
	for (size_t i = 0; i < m_objectsToAdd.size(); i++)
		if (m_objectsToAdd[i]->m_id == id)
			return m_objectsToAdd[i];
	return nullptr;
}