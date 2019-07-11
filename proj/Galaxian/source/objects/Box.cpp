#include "objects/Box.h"

Box::Box(Game & app, const float minX, const float minY, const float maxX, const float maxY, NHTV::SColour color):
	Object(app,"hud","images/box.png",minX + (maxX-minX) * 0.5f,minY+ (maxY - minY) * 0.5f,1,1)
{
	m_collideable = false;
	app.SetSpriteColour(m_id, color);
	app.SetSpriteScale(m_id, (maxX - minX), (maxY - minY));
}
