#pragma once
#include "objects/GalaxianState.h"
#include "objects/Object.h"
#include "Animation.h"

class Group;

class Galaxian : public Object
{
public:

	Galaxian(Game & app, const unsigned int score, const char *textureName, const float x, const float y, 
		const float width = 24, const float height = 24, const bool hasAnimation = true);

	virtual void update(const float elapsedTime) override final;

	virtual void onEvent(const Event event, Object *obj) override;

	virtual GalaxianState* dive()=0;
	
	// The degree is anti clockwise in degrees
	void setRotation(float rotation, bool relativeFromPlayer);

	// This is used to get the player position
	static unsigned int m_playerId;
	static Group * m_group;
	unsigned int m_score;

	// States need access to this to halt animation update
	Animation m_animation;
	bool m_hasAnimation;
	bool m_mayDive;

	virtual void destroy() override final;

protected:

	GalaxianState *m_currentState;
	
private:

	float m_previousRotate;
};