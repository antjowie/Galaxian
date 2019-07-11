#pragma once
#include <vector>

class Game;
class Starfield
{
public:

	Starfield(Game & app);

	void loadStars();
	void update(const float elapsedTime);
	void draw();

private:
	
	std::vector<unsigned int> m_stars;
	std::vector<float> m_starsTime;
	std::vector<float> m_starsHertz;
	std::vector<bool> m_starsActive;

	unsigned m_star1;

	// Needed to move stars
	Game &m_app;
	const float m_speed;
};