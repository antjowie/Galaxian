#include "groups/GalaxianGroup.h"
#include "objects/GalaxianBlue.h"
#include "objects/GalaxianPurple.h"
#include "objects/GalaxianRed.h"
#include "objects/GalaxianYellow.h"

#include <cassert>

GalaxianGroup::GalaxianGroup(Game & app, const float x, const float y):
	Group(app,x,y),
	m_movingRight(true),
	m_speed(80),
	m_elapsedTime(0),
	m_wobbleDuration(0.3f * 4.f),
	m_pattern(new NormalAttack())
{
	for (int i = 0; i < m_xSize; i++)
		m_wobbleTimeline[i] = float(std::rand() % int(m_wobbleDuration * 100.f)) / 100.f;
}

void GalaxianGroup::update(const float elapsedTime)
{
	// Check if still anyone in group alive
	if (getCount() == 0)
		return;


	// Update wobble movement
	for (int i = 0; i < m_xSize; i++)
	{
		m_wobbleTimeline[i] += elapsedTime;
		if (m_wobbleTimeline[i] > m_wobbleDuration)
			m_wobbleTimeline[i] = std::fmod(m_wobbleTimeline[i], m_wobbleDuration);
	}

	std::vector<unsigned int> leftGalaxians = getMostLeftArray();
	std::vector<unsigned int> rightGalaxians = getMostRightArray();

	// Check if bullet of player is inbetween group
	std::vector<Object*> objects = m_app.m_objectManager.getObjects();
	if (std::count_if(objects.begin(), objects.end(), [](Object* obj) {return obj->m_name == "bullet"; }) == 1)
	{

		Object *bullet = *std::find_if(objects.begin(), objects.end(), [](Object* obj) {return obj->m_name == "bullet"; });

		float bX, bY;
		float lX, lY;
		float rX, rY;
		m_app.GetSpritePosition(bullet->m_id,		bX, bY);
		m_app.GetSpritePosition(leftGalaxians[0],	lX, lY);
		m_app.GetSpritePosition(rightGalaxians[0],	rX, rY);

		if (bX == NHTV::Clampf(bX, lX, rX) &&
			bY == NHTV::Clampf(bY, m_y - m_ySize * 0.75f * m_height, m_y + m_ySize * 0.75f * m_height))
			return;
	}
	
	// Update movement
	float x, y;
	if (m_movingRight)
	{
		m_x += m_speed * elapsedTime;

		m_app.GetSpritePosition(rightGalaxians.front(), x, y);
		if (x + m_width > m_app.m_maxX)
			m_movingRight = false;
	}
	else
	{
		m_x -= m_speed * elapsedTime;

		m_app.GetSpritePosition(leftGalaxians.front(), x, y);
		if (x - m_width < m_app.m_minX)
			m_movingRight = true;
	}
	
	// Update attack pattern
	// -----------------------------
	if(static_cast<Galaxian*>(m_app.m_objectManager.getObjectById(getMostLeftArray()[0]))->m_playerId != 0)
	m_pattern->execute(this, elapsedTime);
}

void GalaxianGroup::getPosition(const unsigned int id, float & x, float & y) const
{
	Group::getPosition(id, x, y);

	Galaxian * galax = static_cast<Galaxian*>(m_app.m_objectManager.getObjectById(id));
	if (galax == nullptr|| !galax->m_hasAnimation)
		return;

	size_t row, column;
	findId(id, row, column);

	const float offset = 2;
	const float & time = m_wobbleTimeline[row];

	if (time < m_wobbleDuration* 0.25f)
	{
		x -= offset;
		y += offset;
		galax->m_animation.setAnimation(1);
	}
	else if (time < m_wobbleDuration * 0.5f)
		galax->m_animation.setAnimation(0);
	else if (time < m_wobbleDuration * 0.75f && time > m_wobbleDuration * 0.5f)
	{
		x += offset;
		y += offset;
		galax->m_animation.setAnimation(1);
	}
	else
		galax->m_animation.setAnimation(0);

}

Object * GalaxianGroup::numberToObject(const unsigned short number) const
{
	switch (number)
	{
	case 1:
		return new GalaxianBlue(m_app, m_x, m_y);
	case 2:
		return new GalaxianPurple(m_app, m_x, m_y);
	case 3:
		return new GalaxianRed(m_app, m_x, m_y);
	case 4:
		return new GalaxianYellow(m_app, m_x, m_y);
	default:
		assert(false && "Number not found");
		return nullptr;
	}
}

// Attack Patterns
//------------------------------

NormalAttack::NormalAttack():
	m_attackInterval(0),
	m_attackCount(0),
	m_attackTimeline(0)
{
}

void MadAttack::execute(GalaxianGroup * group, const float elapsedTime)
{
	if (m_attackTimeline < 5.f)
	{
		m_attackTimeline += elapsedTime;
		return;
	}

	if (m_attackInterval <= 0.5f)
	{
		m_attackInterval += elapsedTime;
		return;
	}

	m_attackCount++;
	m_attackInterval = 0;

	// Reset attack timers
	if (m_attackCount > 9)
	{
		m_attackTimeline = 0.f;
		m_attackCount = 0;
		return;
	}

	// Send two normal Galaxians
	if (m_attackCount <= 7)
	{
		std::vector<unsigned int> leftGalaxians = group->getMostLeftArray();
		std::vector<unsigned int> rightGalaxians = group->getMostRightArray();

		// Merge vectors
		leftGalaxians.insert(leftGalaxians.end(), rightGalaxians.begin(), rightGalaxians.end());

		// Send some random Galaxians to dive
		Galaxian *galaxian = static_cast<Galaxian*>(
			group->m_app.m_objectManager.getObjectById(leftGalaxians[std::rand() % leftGalaxians.size()]));
		if (galaxian)
			galaxian->m_mayDive = true;
	}
	else // Send three galaxians in formation
	{
		std::vector<unsigned int> topGalaxians;

		// Get the commanders
		for (int x = 0; x < group->m_xSize; x++)
		{
			if (group->m_map[x][0] != 0)
				topGalaxians.push_back(group->m_map[x][0]);
		}

		if (topGalaxians.size() == 0)
			return;

		unsigned int id = topGalaxians[std::rand() % topGalaxians.size()];

		size_t x, y;
		group->findId(id, x, y);

		bool left = x < group->m_xSize*0.5f;

		// Get red galaxians
		std::vector<unsigned int> aroundCommander;
		for (int i = -1; i <= 1; i++)
			if (group->m_map[x + i][1] != 0)
				aroundCommander.push_back(group->m_map[x + i][1]);

		if (aroundCommander.size() == 3)
		{
			if (left)
				aroundCommander.pop_back();
			else
			{
				unsigned front = aroundCommander[0];
				aroundCommander[0] = aroundCommander[2];
				aroundCommander[2] = front;
				aroundCommander.pop_back();
			}
		}

		Galaxian* galax = static_cast<Galaxian*>(group->m_app.m_objectManager.getObjectById(id));
		if (galax)
			galax->m_mayDive = true;
		for (int i = 0; i < aroundCommander.size(); i++)
		{
			Galaxian* galax = static_cast<Galaxian*>(group->m_app.m_objectManager.getObjectById(aroundCommander[i]));
			if (galax)
				galax->m_mayDive = true;
		}
	}
}

MadAttack::MadAttack() :
	m_attackInterval(0),
	m_attackCount(0),
	m_attackTimeline(0)
{
}

void NormalAttack::execute(GalaxianGroup * group, const float elapsedTime)
{
	if (m_attackTimeline < 3.f)
	{
		m_attackTimeline += elapsedTime;
		return;
	}

	if (m_attackInterval <= 1.f)
	{
		m_attackInterval += elapsedTime;
		return;
	}

	m_attackCount++;
	m_attackInterval = 0;

	// Reset attack timers
	if (m_attackCount > 3)
	{
		m_attackTimeline = 0.f;
		m_attackCount = 0;
		return;
	}

	// Send two normal Galaxians
	if (m_attackCount <= 2)
	{
		std::vector<unsigned int> leftGalaxians = group->getMostLeftArray();
		std::vector<unsigned int> rightGalaxians = group->getMostRightArray();

		// Merge vectors
		leftGalaxians.insert(leftGalaxians.end(), rightGalaxians.begin(), rightGalaxians.end());

		// Send some random Galaxians to dive
		Galaxian *galaxian = static_cast<Galaxian*>(
			group->m_app.m_objectManager.getObjectById(leftGalaxians[std::rand() % leftGalaxians.size()]));
		if (galaxian)
			galaxian->m_mayDive = true;
	}
	else // Send three galaxians in formation
	{
		std::vector<unsigned int> topGalaxians;

		// Get the commanders
		for (int x = 0; x < group->m_xSize; x++)
		{
			if (group->m_map[x][0] != 0)
				topGalaxians.push_back(group->m_map[x][0]);
		}

		if (topGalaxians.size() == 0)
			return;

		unsigned int id = topGalaxians[std::rand() % topGalaxians.size()];

		size_t x, y;
		group->findId(id, x, y);

		bool left = x < group->m_xSize*0.5f;

		// Get red galaxians
		std::vector<unsigned int> aroundCommander;
		for (int i = -1; i <= 1; i++)
			if (group->m_map[x + i][1] != 0)
				aroundCommander.push_back(group->m_map[x + i][1]);

		if (aroundCommander.size() == 3)
		{
			if (left)
				aroundCommander.pop_back();
			else
			{
				unsigned front = aroundCommander[0];
				aroundCommander[0] = aroundCommander[2];
				aroundCommander[2] = front;
				aroundCommander.pop_back();
			}
		}

		Galaxian* galax = static_cast<Galaxian*>(group->m_app.m_objectManager.getObjectById(id));
		if (galax)
			galax->m_mayDive = true;
		for (int i = 0; i < aroundCommander.size(); i++)
		{
			Galaxian* galax = static_cast<Galaxian*>(group->m_app.m_objectManager.getObjectById(aroundCommander[i]));
			if (galax)
				galax->m_mayDive = true;
		}
	}
}
