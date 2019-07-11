#include "objects/Bullet.h"

Bullet::Bullet(Game & app, float x, float y, float speed, const char* name, NHTV::SColour &colour) :
	Object(app, name, "./images/verticalline.png", x, y,1,6,8,8),
	m_isShot(false),
	m_speed(speed)
{
	m_app.SetSpriteColour(m_id, colour);
	m_collideable = false;
	m_app.RotateSprite(m_id, -90);
}

void Bullet::update(const float elapsedTime)
{
	if (!m_isShot) return;

	float x, y;
	m_app.GetSpritePosition(m_id, x, y);

	// Check if bullet is OOB
	if (y > m_app.m_maxY ||
		y < m_app.m_minY)
	{
		destroy();
		return;
	}
	
	// Calculate unit vector in the right direction
	m_app.MoveSprite(m_id, x + m_unitX * m_speed * elapsedTime, y + m_unitY * m_speed * elapsedTime);
}

void Bullet::destroy()
{
	Object::destroy();
	m_app.m_eventManager.pushEvent(Event::Bullet_Destroyed, this);
}

void Bullet::onEvent(const Event event, Object * obj)
{
	switch (event)
	{
	case Event::Collision:
		if (obj->m_name == "galaxian")
			destroy();
		break;
	}
}

void Bullet::shoot(float targetX, float targetY)
{
	m_isShot = true;
	m_collideable = true;
	
	float x, y;
	m_app.GetSpritePosition(m_id, x, y);
	m_unitX = targetX - x;
	m_unitY = targetY - y;

	// Bullets cannot go too angled
	m_unitX = NHTV::Clampf(m_unitX, -50, 50);

	float magnitude = std::sqrtf(std::powf(m_unitX, 2) + std::powf(m_unitY, 2));
	m_unitX /= magnitude;
	m_unitY /= magnitude;

	m_app.RotateSprite(m_id, -std::atan2f(m_unitX, m_unitY)* NHTV::RAD2DEG);
}

GalaxianBullet::GalaxianBullet(Game & app, float x, float y, float speed):
	Bullet(app, x, y, speed, "galaxianBullet", NHTV::SColour(255,255,255,255))
{
}

void GalaxianBullet::onEvent(const Event event, Object * obj)
{
	switch (event)
	{
	case Event::Collision:
		if (obj->m_name == "ship")
			destroy();
			break;
	}
}
