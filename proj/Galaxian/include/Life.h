#pragma once

class Game;
class Life
{
public:

	Life(Game &app);
	~Life();

	void resetLife();
	void popLife();
	void setDrawPosition(const float x, const float y);
	void draw() const;

	int getLifeCount() const;

private:

	int m_lifeCount;
	unsigned int m_lifeSprites[3];
	float m_x, m_y;
	float m_width, m_height;

	Game &m_app;
};