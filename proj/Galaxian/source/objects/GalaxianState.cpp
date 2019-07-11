#include "objects/GalaxianState.h"
#include "objects/Galaxian.h"
#include "groups/Group.h"

#include "Utilities.h"

GalaxianState * GalaxianFormation::update(Galaxian *obj, const float elapsedTime)
{
	float x = 0, y = 0;

	obj->m_group->getPosition(obj->m_id, x, y);
	obj->m_app.MoveSprite(obj->m_id, x, y);
	obj->setRotation(0, false);

	if (obj->m_hasAnimation)
	{
		obj->m_animation.update(elapsedTime);

		float minU, maxU;
		float minV, maxV;

		// Update animation
		obj->m_animation.getCurrentAnimation(minU, minV, maxU, maxV);
		obj->m_app.SetSpriteUVCoordinates(obj->m_id, minU, minV, maxU, maxV);
	}
	
	if (obj->m_mayDive)
		return obj->dive();

	return nullptr;
}

GalaxianDive::GalaxianDive(Galaxian* obj, size_t row, size_t column,const float diveDuration):
	m_curve(diveDuration),
	m_row(row),m_column(column),
	m_shootCount(3),
	m_shootTimeline(2)
{
}

GalaxianState * GalaxianDive::update(Galaxian * obj, const float elapsedTime)
{
	// Update curve pos
	m_curve.update(elapsedTime);

	// Move sprite
	float x, y;
	m_curve.getPosition(x, y);
	obj->m_app.MoveSprite(obj->m_id,x, y);

	if (obj->m_playerId != 0)
	{


	// Update rotation to face player
	obj->setRotation(180, true);

	// Check if can shoot
	if (m_shootCount > 0 && y <= obj->m_app.m_maxY * 0.5f)
	{
		m_shootTimeline += elapsedTime;
		if (m_shootTimeline > 0.25f)
		{
			m_shootTimeline = 0;
			m_shootCount--;

			float tX, tY;
			obj->m_app.GetSpritePosition(obj->m_playerId, tX, tY);

			(new GalaxianBullet(obj->m_app, x,y-16,400))->shoot(tX,tY);
		}
	}
	}

	// Check if outside of borders
	if( x - obj->m_width * 0.5f > obj->m_app.m_maxX ||
		x + obj->m_width * 0.5f < obj->m_app.m_minX ||
		y + obj->m_height * 0.5f < obj->m_app.m_minY)
	{
		return new GalaxianReturnToFormation(obj,m_row,m_column);
	}
	return nullptr;
}

GalaxianAggressiveDive::GalaxianAggressiveDive(Galaxian *obj, size_t row, size_t column):
	GalaxianDive(obj,row,column,4.f)
{
	// Calculate curve control points
	float tX, tY; // Target coordinates
	float gX, gY; // Galax  coordinates
	obj->m_app.GetSpritePosition(Galaxian::m_playerId, tX, tY);
	obj->m_app.GetSpritePosition(obj->m_id, gX, gY);

	// -16 is so that the sprite actually goes outside the screen if it reaches end of destination
	const float curveOffset = NHTV::Clampf(tX - gX,-1,1) * (obj->m_app.m_maxX - obj->m_app.m_minX) * 0.5f;
	//const float curveOffset = (gX > tX ? 1 : -1) * (obj->m_app.m_maxX - obj->m_app.m_minX) * 0.5f;

	// Fake AI prediction
	float targetOffset = 0;
	switch (std::rand() % 3)
	{
	case 0:
		targetOffset = -1;
		break;
	case 1:
		targetOffset = 0;
		break;
	case 2:
		targetOffset = 1;
		break;
	}

	m_curve.setControlPoints(gX, gY,tX + curveOffset, gY*0.5f, tX - NHTV::Clampf(curveOffset,-1,1) * 64.f + 128.f * targetOffset, -16);
}

GalaxianReturnToFormation::GalaxianReturnToFormation(Galaxian *obj, size_t row, size_t column):
	m_elapsedTime(0),
	m_timeTillFormation(1.5f),
	m_row(row),m_column(column)
{
	obj->m_mayDive = false;
}

GalaxianState * GalaxianReturnToFormation::update(Galaxian * obj, const float elapsedTime)
{
	// Get the position
	float x,y;
	obj->m_group->addToGroup(obj->m_id, m_row, m_column);
	obj->m_group->getPosition(obj->m_id, x, y);

	m_elapsedTime += elapsedTime / m_timeTillFormation;

	obj->m_app.MoveSprite(obj->m_id, x, NHTV::Lerp(obj->m_app.m_maxY + 32,y,m_elapsedTime));

	// Update rotation if needed
	const float rotate = NHTV::Clampf(m_elapsedTime - 0.75f, 0, 0.25f);
	if (rotate == 0)
		obj->setRotation(180, false);
	else
		obj->setRotation(180 + rotate * 4.f*180.f,false);

	// Reached formation
	if (m_elapsedTime > 1.f)
		return new GalaxianFormation();

	obj->m_group->removeFromGroup(obj->m_id);
	return nullptr;
}

GalaxianLoop::GalaxianLoop(Galaxian * obj):
	m_curve(1.f)
{
	// When sprite attacks, animation does not continue
	obj->m_app.SetSpriteUVCoordinates(obj->m_id, 0, 0, 0.5, 1);
	if (!obj->m_hasAnimation)
		obj->m_app.SetSpriteUVCoordinates(obj->m_id, 0, 0, 1, 1);

	// The Galaxian is removed from the group, so that the group still
	// bounces at the right moment
	obj->m_group->findId(obj->m_id, m_row, m_column);
	obj->m_group->removeFromGroup(obj->m_id);

	float gX, gY;
	float tX, tY;

	obj->m_app.GetSpritePosition(obj->m_id, gX, gY);
	obj->m_app.GetSpritePosition(obj->m_playerId, tX, tY);
	const float offset = 32.f;
	m_direction = gX > tX ? 1 : -1;

	// Check if Galaxian will not leave screen during loop
	m_direction = (gX + offset * m_direction*2.f) == NHTV::Clampf(gX + offset * m_direction*2.f, obj->m_app.m_minX, obj->m_app.m_maxX) ? m_direction : -m_direction;

	m_curve.setControlPoints(gX, gY, gX + offset*m_direction, gY + offset* 3.f, gX + offset*m_direction*2.f, gY);
}

GalaxianState * GalaxianLoop::update(Galaxian * obj, const float elapsedTime)
{
	m_curve.update(elapsedTime);
	
	float x, y;
	m_curve.getPosition(x, y);
	

	obj->m_app.MoveSprite(obj->m_id, x, y);
	obj->setRotation(-m_direction * m_curve.getProgress() * 180.f - m_direction * 40,false);
	if (m_curve.getProgress() > 0.80f)
		obj->setRotation(180, true);

	if (m_curve.getProgress() >= 1.f)
		return getState(obj);
	else if (x + 16 < obj->m_app.m_minX || x - 16 > obj->m_app.m_maxX)
		return new GalaxianReturnToFormation(obj,m_row,m_column);
	
	return nullptr;
}

GalaxianNormalLoop::GalaxianNormalLoop(Galaxian * obj):
	GalaxianLoop(obj)
{
}

GalaxianState * GalaxianNormalLoop::getState(Galaxian * obj)
{
	return new GalaxianAggressiveDive(obj,m_row,m_column);
}
