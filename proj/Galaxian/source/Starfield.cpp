#include "Starfield.h"
#include "Game.h"

Starfield::Starfield(Game &app):
	m_speed(64),
	m_app(app)
{
}

void Starfield::loadStars()
{
	// Let's spawn 3 stars randomly in a area of 50*50 pixels
	const float width = 100.f;
	const float height = 100.f;

	for (int x = m_app.m_minX; x < m_app.m_maxX; x += width)
		for (int y = m_app.m_minY; y < m_app.m_maxY; y += height)
			for (int i = 0; i < 3; i++)
			{
				// Random position
				const float starX = x + std::rand() % (int)width;
				const float starY = y + std::rand() % (int)height;

				// Random bomb frame
				const float u = float(std::rand() % 2 + 1) * 0.5f;

				// Random color
				NHTV::SColour color;
				color.argb.colours.a = std::rand() % 155 + 100;
				color.argb.colours.r = std::rand() % 155 + 100;
				color.argb.colours.g = std::rand() % 155 + 100;
				color.argb.colours.b = std::rand() % 155 + 100;

				unsigned id = m_app.CreateSprite("images/bomb.png", 16, 8, true, color);
				m_app.SetSpriteScale(id, std::rand() % 4 + 4, std::rand() % 4 + 4);
				m_app.MoveSprite(id, starX, starY);
				m_app.SetSpriteUVCoordinates(id, u - 0.5f, 0.f, u, 1.f);

				m_stars.push_back(id);
				m_starsTime.push_back(0);
				m_starsHertz.push_back(float(std::rand() % 100) / 100.f);
				m_starsActive.push_back(false);
			}
}

void Starfield::update(const float elapsedTime)
{
	const float yMove = m_speed * elapsedTime;
	
	for (int i = 0; i < m_stars.size(); i++)
	{
		float x, y;
		m_app.GetSpritePosition(m_stars[i], x, y);
		y -= yMove;
		if (y < -10)
			y = m_app.m_maxY + 10;
		m_app.MoveSprite(m_stars[i], x, y);
	
		m_starsTime[i] += elapsedTime * m_starsHertz[i];
		if (m_starsTime[i] > 1.f)
		{
			m_starsActive[i] = m_starsActive[i] ? false : true;
			m_starsTime[i] = 0;
		}

	}
}

void Starfield::draw()
{
	for (int i = 0; i < m_stars.size(); i++)
		if(m_starsActive[i])
			m_app.DrawSprite(m_stars[i]);
}