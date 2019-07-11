#include "objects/Galaxian.h"
#include "objects/Ship.h"
#include "objects/Explosion.h"
#include "groups/GalaxianGroup.h"

#include "Utilities.h"

unsigned int Galaxian::m_playerId;
Group *Galaxian::m_group = nullptr;

Galaxian::Galaxian(Game & app, const unsigned int score, const char* textureName, const float x, const float y, const float width, const float height, const bool hasAnimation) :
	Object(app, "galaxian", textureName, x, y, width, height, 32, 32), 
	m_animation(2,10.f),
	m_previousRotate(0),
	m_hasAnimation(hasAnimation),
	m_currentState(new GalaxianFormation()),
	m_mayDive(false),
	m_score(score)
{
	m_app.SetSpriteScale(m_id, width, height);
}

void Galaxian::update(const float elapsedTime)
{
	GalaxianState *next = m_currentState->update(this, elapsedTime);
	if (next)
	{
		delete m_currentState;
		m_currentState = next;
		next = nullptr;
	}
}

void Galaxian::onEvent(const Event event, Object * obj)
{
	switch (event)
	{
	case Event::Collision:
		if (obj->m_name == "ship" || obj->m_name == "bullet")
		{
			m_group->removeFromGroup(m_id);
			
			// Spawn explosion
			float x, y;
			m_app.GetSpritePosition(m_id, x, y);
			new Explosion(m_app, x, y);

			destroy();
		}
		break;
	}
}

void Galaxian::setRotation(float rotation, bool relativeFromPlayer)
{
	float degrees = static_cast<float>(std::fmod(rotation, 360));

	if (relativeFromPlayer)
	{
		// Update rotation
		float pX, pY;
		float x, y;
		m_app.GetSpritePosition(m_playerId, pX, pY);
		m_app.GetSpritePosition(m_id, x, y);

		float rotate = NHTV::RAD2DEG * std::atan2(pX - x, y - pY);

		degrees += rotate;
	}

	degrees = static_cast<float>((int)degrees - (int)degrees % 10);

	m_app.RotateSprite(m_id, -m_previousRotate);

	m_previousRotate = degrees;
	m_app.RotateSprite(m_id, degrees);
}

void Galaxian::destroy()
{
	Object::destroy();
	if (m_mayDive)
		m_score *= 2;
	m_app.m_eventManager.pushEvent(Event::Galaxian_Destroyed, this);
}
