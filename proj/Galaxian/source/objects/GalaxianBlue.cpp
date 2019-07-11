#include "objects/GalaxianBlue.h"
#include "groups/GalaxianGroup.h"

GalaxianBlue::GalaxianBlue(Game &app, const float x, const float y):
	Galaxian(app, 30, "./images/galaxianA.png",x,y)
{
	m_currentState = new GalaxianFormation();
}

GalaxianState * GalaxianBlue::dive()
{
	return new GalaxianBlueLoop(this);
}

GalaxianBlueLoop::GalaxianBlueLoop(Galaxian * obj) :
	GalaxianLoop(obj)
{
}

GalaxianState * GalaxianBlueLoop::getState(Galaxian * obj)
{
	if (obj->m_group->getCount() < 10)
		return new GalaxianAggressiveDive(obj,m_row,m_column);
	else
		return new GalaxianNormalDive(obj,m_row,m_column);

}

GalaxianNormalDive::GalaxianNormalDive(Galaxian *obj, size_t row, size_t column) :
	GalaxianDive(obj,row,column,3.f)
{
	// Calculate curve control points
	float tX, tY; // Target coordinates
	float gX, gY; // Galax  coordinates
	obj->m_app.GetSpritePosition(Galaxian::m_playerId, tX, tY);
	obj->m_app.GetSpritePosition(obj->m_id, gX, gY);

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

	// -16 is so that the sprite actually goes outside the screen if it reaches end of destination
	m_curve.setControlPoints(gX, gY, tX, gY*0.5f, tX + 64.f * targetOffset, -16);
}
