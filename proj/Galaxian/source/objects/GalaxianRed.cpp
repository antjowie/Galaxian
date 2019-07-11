#include "objects/GalaxianRed.h"

GalaxianRed::GalaxianRed(Game &app, const float x, const float y) :
	Galaxian(app, 50,"./images/galaxianB.png", x, y)
{
	m_currentState = new GalaxianFormation();
	m_app.SetSpriteScale(m_id, 24, 24);
}

GalaxianState * GalaxianRed::dive()
{
	return new GalaxianNormalLoop(this);
}
