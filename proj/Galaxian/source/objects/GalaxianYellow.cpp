#include "objects/GalaxianYellow.h"

GalaxianYellow::GalaxianYellow(Game &app, const float x, const float y) :
	Galaxian(app,150, "./images/galaxianD.png", x, y,32,32,false)
{
	m_currentState = new GalaxianFormation();
	const float scale = 32.f / 11.f;
	m_app.SetSpriteScale(m_id, 32, 32);
}

GalaxianState *GalaxianYellow::dive()
{
	return new GalaxianNormalLoop(this);
}
