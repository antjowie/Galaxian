#include "CollisionManager.h"
#include "Game.h"
#include "EventManager.h"
#include "objects/Object.h"

CollisionManager::CollisionManager(Game & app):
	m_app(app)
{
}

void CollisionManager::resolveCollision()
{
	for (std::vector<Object*>::iterator obj1 = m_app.m_objectManager.getObjects().begin();
		obj1 != m_app.m_objectManager.getObjects().end(); obj1++)
	{
		if (!(*obj1)->m_collideable) continue;
		for (std::vector<Object*>::iterator obj2 = m_app.m_objectManager.getObjects().begin();
			obj2 != m_app.m_objectManager.getObjects().end(); obj2++)
		{
			if (!(*obj2)->m_collideable || obj1 == obj2) continue;
			
			if (intersect(*obj1, *obj2))
			{
				(*obj1)->onEvent(Event::Collision, *obj2);
				(*obj2)->onEvent(Event::Collision, *obj1);
			}	
		}
	}
}

bool CollisionManager::intersect(const Object * const obj1, const Object * const obj2) const
{
	// Make two boxes
	// Sprite positions are always from center
	// so we have to move the positions a bit
	float x1, x2, y1, y2;
	float w1, w2, h1, h2;

	// Get all the needed variables to create the boxes
	m_app.GetSpritePosition(obj1->m_id, x1, y1);
	m_app.GetSpritePosition(obj2->m_id, x2, y2);
	w1 = 0.5f * obj1->m_width;
	w2 = 0.5f * obj2->m_width;
	h1 = 0.5f * obj1->m_height;
	h2 = 0.5f * obj2->m_height;

	// With the boxes now created, we can check for collision
	// Check collision for vertical axis, then horizontal
	if (std::abs(y1 - y2) >= h1 + h2) return false;
	if (std::abs(x1 - x2) >= w1 + w2) return false;

	return true;
}
