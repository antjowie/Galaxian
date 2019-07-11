#include "objects/GalaxianPurple.h"

GalaxianPurple::GalaxianPurple(Game &app, const float x, const float y) :
	Galaxian(app, 40, "./images/galaxianC.png", x, y)
{
	m_currentState = new GalaxianFormation();
	m_app.SetSpriteScale(m_id, 24, 24);
}

GalaxianState * GalaxianPurple::dive()
{
	return new GalaxianNormalLoop(this);
}
