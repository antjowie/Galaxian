#pragma once

// Curve is Bèzier curve class. I learned about this in the workshop from David
// about Maths for games
class Curve
{
public:

	Curve(const float duration, float x1, float y1, float x2, float y2, float x3, float y3);
	Curve(const float duration);

	void update(const float elapsedTime);

	void setControlPoints(float x1, float y1, float x2, float y2, float x3, float y3);
	void getPosition(float &spriteX, float &spriteY) const;

	float getProgress() const;

private:

	float m_x1, m_x2, m_x3;
	float m_y1, m_y2, m_y3;

	float m_dt;
	const float m_duration;
};