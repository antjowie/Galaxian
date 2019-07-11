#include "Life.h"
#include "Game.h"

Life::Life(Game &app) :
	m_lifeCount(0),
	m_app(app),
	m_x(-100),
	m_y(-100),
	m_width(32),
	m_height(32)
{
}

Life::~Life()
{
	for (int i = 0; i < m_lifeCount; i++)
		m_app.DestroySprite(m_lifeSprites[i]);
}

void Life::resetLife()
{
	// Clear old sprites
	int lifeCount = m_lifeCount;
	for (int i = 0; i < lifeCount; i++)
		popLife();
	m_lifeCount = 3;

	// Make new sprites
	for (int i = 0; i < m_lifeCount; i++)
	{
		m_lifeSprites[i] = m_app.CreateSprite("images/shooter.png", 32,32);
		m_app.MoveSprite(m_lifeSprites[i], m_x + m_width * i* 1.5f, 50);
	}
}

void Life::popLife()
{
	m_lifeCount--;
	m_app.DestroySprite(m_lifeSprites[m_lifeCount]);
}

void Life::setDrawPosition(const float x, const float y)
{
	m_x = x;
	m_y = y;
}

void Life::draw() const
{
	for (int i = 0; i < m_lifeCount; i++)
		m_app.DrawSprite(m_lifeSprites[i]);
}

int Life::getLifeCount() const
{
	return m_lifeCount;
}
