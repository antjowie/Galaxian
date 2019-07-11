#pragma once
#include "Object.h"
#include "Animation.h"

class Explosion : public Object
{
public:

	Explosion(Game & app, const float x, const float y, bool isShip = false);

	virtual void update(const float elapsedTime) override final;

private:

	Animation m_animation;
	
	float m_elapsedTime;
	const float m_lifeTime;
};