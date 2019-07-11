#pragma once
#include "objects/Object.h"

class Bullet : public Object
{
public:

	Bullet(Game &app, float x, float y, float speed, const char* name = "bullet", NHTV::SColour &colour = NHTV::SColour(255,255,0,255));

	virtual void update(const float elapsedTime) override final;
	virtual void destroy() override final;
	virtual void onEvent(const Event event, Object *obj) override;

	void shoot(float targetX, float targetY);

private:

	float m_unitX, m_unitY;
	const float m_speed;

	bool m_isShot;
};

class GalaxianBullet : public Bullet
{
public:
	GalaxianBullet(Game &app, float x, float y, float speed);

	virtual void onEvent(const Event event, Object *obj) override final;
};