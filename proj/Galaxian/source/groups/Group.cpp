#include "groups/Group.h"
#include "objects/Galaxian.h"

Group::Group(Game & app, const float x, const float y):
	m_app(app),
	m_width(0),
	m_height(0),
	m_x(x),
	m_y(y), 
	m_spawnX(x),
	m_spawnY(y),
	m_xSize(10),
	m_ySize(6)
{
	for (size_t x = 0; x < m_xSize; x++)
		for (size_t y = 0; y < m_ySize; y++)
			m_map[x][y] = 0;
}

void Group::loadGroup(const unsigned int map[6][10])
{
	clear();

	// Convert layout of the map
	//
	//   1 2 3	> 1|2|3
	//  =======	>  | |
	//	 4 5 6	> 4|5|6
	//
	for (size_t x = 0; x < m_xSize; x++)
		for (size_t y = 0; y < m_ySize; y++)
		{
			if (map[y][x] != 0)
			{
				int id = numberToObject(map[y][x])->m_id;
				m_map[x][y] = id;
				float xPos = 0, yPos = 0;
				
				m_app.m_objectManager.getObjectById(id)->update(0);
			}
			else
			{
				m_map[x][y] = 0;
			}
		}
}

void Group::getPosition(const unsigned int id, float & x, float & y) const
{
	size_t row, column;
	findId(id, row, column);
	if (row == -1 && column == -1)
	{
		x = y = -1;	
	}
	else
	{
		// If we have 7 columns of objects, they need an offset of half the cell
		float xOffset = (m_xSize % 2) * (m_width * 0.5f);
		//const float yOffset = (m_map.size() % 2) * (m_width * 0.5f);

		// size_t can't be negative
		int iRow = (int)row, iColumn = (int)column;
		iRow -= static_cast<int>(m_xSize * 0.5f);
		iColumn -= static_cast<int>(m_ySize * 0.5f);

		xOffset = iRow < 0 ? -xOffset : xOffset;

		x = m_x + (iRow * m_width + xOffset);
		// The upper side of the array is supposed to be positive, do we flip it around
		y = m_y - (iColumn * m_height);
	}
}

void Group::setCellSize(const float width, const float height)
{
	m_width = width;
	m_height = height;
}

void Group::removeFromGroup(const unsigned int id)
{
	size_t row, column;
	findId(id, row, column);
	if (row != -1 && column != -1)
		m_map[row][column] = 0;
}

void Group::addToGroup(const unsigned int id, const unsigned int x, const unsigned int y)
{
	m_map[x][y] = id;
}

void Group::findId(const unsigned int id, size_t & row, size_t & column) const
{
	for (row = 0; row < m_xSize; row++)
		for (column = 0; column < m_ySize; column++)
			if (m_map[row][column] == id)
				return;

	row = column = -1;
}

unsigned int Group::getCount() const
{
	unsigned int count = 0;
	for (int x = 0; x < m_xSize; x++)
		for (int y = 0; y < m_ySize; y++)
			if (m_map[x][y] != 0)
				count++;

	return count;
}

void Group::clear()
{
	for (int x = 0; x < m_xSize; x++)
		for (int y = 0; y < m_ySize; y++)
			if (m_map[x][y] != 0)
				m_map[x][y] = 0;
}

void Group::moveToOriginal()
{
	// Replace to middle or original spawn
	m_x = m_spawnX;
	m_y = m_spawnY;
}

std::vector<unsigned>  Group::getMostRightArray() const
{
	std::vector<unsigned> elements;
	size_t x;
	// Careful with iterating an array backwards
	for (x = m_xSize -1; x >= 0; x--) // Yep, only this line is different
		for (size_t y = 0; y < m_ySize; y++)
			if (m_map[x][y] != 0)
			{
				for (; y < m_ySize; y++)
					if (m_map[x][y] != 0)
						elements.push_back(m_map[x][y]);
				return elements;
			}
	return elements;
}

std::vector<unsigned>  Group::getMostLeftArray () const
{
	std::vector<unsigned> elements;
	size_t x;
	for (x = 0; x < m_xSize; x++) // Yep, only this line is different
		for (size_t y = 0; y < m_ySize; y++)
			if (m_map[x][y] != 0 && m_map[x][y] != 0)
			{
				for (; y < m_ySize; y++)
					if (m_map[x][y] != 0)
						elements.push_back(m_map[x][y]);
				return elements;
			}
	return elements;
}
