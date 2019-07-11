#include "Curve.h"
#include "Utilities.h"

Curve::Curve(const float duration, float x1, float y1, float x2, float y2, float x3, float y3):
	m_dt(0),
	m_duration(duration)
{
	setControlPoints(x1, y1, x2, y2, x3, y3);
}

Curve::Curve(const float duration) :
	Curve(duration,0,0,0,0,0,0)
{
}

void Curve::update(const float elapsedTime)
{
	m_dt += elapsedTime / m_duration;
}

void Curve::setControlPoints(float x1, float y1, float x2, float y2, float x3, float y3)
{
	m_x1 = x1;
	m_x2 = x2;
	m_x3 = x3;
	m_y1 = y1;
	m_y2 = y2;
	m_y3 = y3;
}

void Curve::getPosition(float & spriteX, float & spriteY) const
{
	// Get the lerps of the lines, hopefully lerps are not too expensives
	const float xLerp1 = NHTV::Lerp(m_x1, m_x2, m_dt);
	const float yLerp1 = NHTV::Lerp(m_y1, m_y2, m_dt);
	const float xLerp2 = NHTV::Lerp(m_x2, m_x3, m_dt);
	const float yLerp2 = NHTV::Lerp(m_y2, m_y3, m_dt);

	spriteX = NHTV::Lerp(xLerp1, xLerp2, m_dt);
	spriteY = NHTV::Lerp(yLerp1, yLerp2, m_dt);
}

float Curve::getProgress() const
{
	return m_dt;
}
