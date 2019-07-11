#include "objects/Object.h"
#include <iostream>

Object::Object(Game & app, const char * name, const char *textureName, 
	const float x, const float y, const float width, const float height, 
	const float textureWidth, const float textureHeight):
	m_app(app),
	m_name(name),
	m_width(width),
	m_height(height),
	m_dead(false),
	m_collideable(true),
	m_id(m_app.CreateSprite(textureName, 
		textureWidth == -1 ? width : textureWidth, 
		textureHeight == -1 ? height: textureHeight))
{
	m_app.MoveSprite(m_id, x, y);
	m_app.m_objectManager.push(this);
	std::printf("Created a %s\n", name);
}

Object::~Object()
{
	std::printf("Destroyed a %s\n", m_name);
	m_app.DestroySprite(m_id);
}

void Object::update(const float elapsedTime)
{
}

void Object::destroy()
{
	m_dead = true;
	m_collideable = false;
}

void Object::onEvent(const Event event, Object * object)
{
}
