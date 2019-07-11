#pragma once
#include "Game.h"
#include "EventManager.h"

// All objects should be dynamically allocated
class Object
{
public:
	friend ObjectManager;

	Object(Game &app, 
		const char *name, const char* textureName,
		const float x, const float y,
		const float width, const float height,
		const float textureWidth = -1, const float textureHeight = -1);
	virtual ~Object();

	virtual void update(const float elapsedTime);

	// When objects are destroyed, we may want to do something with it like
	// playing an explosion animation. 
	virtual void destroy();

	virtual void onEvent(const Event event, Object* object);

	bool m_collideable;
	const char* m_name;

	const unsigned int m_id;

	const float m_width;
	const float m_height;

	Game &m_app;

private:

	bool m_dead;
};