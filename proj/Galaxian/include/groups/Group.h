#pragma once
#include "Game.h"
#include "objects/Object.h"
#include <array>

class Object;

// !!! Important to know is that the map goes from columns to rows
// !!! from vertical to horizontal. First up down, then left right
// !!! This is to make implementation of update easier
class Group
{
public:

	Group(Game &app,const float x, const float y);

	virtual void update(const float elapsedTime) = 0;
	
	// Load group like it will be displayed in game
	void loadGroup(const unsigned int map[6][10]);
	
	virtual void getPosition(const unsigned int id, float &x, float &y) const;
	void setCellSize(const float width, const float height);

	void removeFromGroup(const unsigned int id);
	void addToGroup(const unsigned int id, const unsigned int x, const unsigned int y);

	void findId(const unsigned int id, size_t &x, size_t &y) const;

	unsigned int getCount() const;

	void clear();
	void moveToOriginal();

	Game &m_app;
	
	std::vector<unsigned int> getMostRightArray() const;
	std::vector<unsigned int> getMostLeftArray() const;

	unsigned int m_map[10][6];
	const size_t m_xSize;
	const size_t m_ySize;

	float m_x, m_y;

	float m_width, m_height;

private:

	virtual Object* numberToObject(const unsigned short number) const = 0;
	
	const float m_spawnX, m_spawnY;
};