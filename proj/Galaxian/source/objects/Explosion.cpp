#include "objects\Explosion.h"

Explosion::Explosion(Game & app, const float x, const float y, bool isShip):
	Object(app,"explosion", isShip ? "images/explosionPlayer.png" : "images/explosionEnemy.png",x,y,32,32),
	m_animation(4,0.08f),
	m_lifeTime(4*0.08f),
	m_elapsedTime(0)
{
	m_collideable = false;
	app.SetSpriteScale(m_id, 32, 32);
}

void Explosion::update(const float elapsedTime)
{
	m_elapsedTime += elapsedTime;

	// Update animation
	m_animation.update(elapsedTime);
	
	float minU, minV,maxU,maxV;
	m_animation.getCurrentAnimation(minU, minV, maxU, maxV);
	m_app.SetSpriteUVCoordinates(m_id, minU, minV, maxU, maxV);

	if (m_elapsedTime > m_lifeTime)
		destroy();
}
