#include "Text.h"
#include "Game.h"

Game *Text::m_app;
std::unordered_map<char, unsigned int>	Text::m_indexes;
std::vector<Text::DrawCall>				Text::m_drawCalls;
unsigned int Text::m_spriteId;
unsigned int Text::m_width;

void Text::draw(const std::string &text, const float x, const float y, const NHTV::SColour colour)
{
	size_t begin = 0, end = 0;
	int containsSpaces = 0;
	for (size_t iter = 0; iter < text.size(); iter++)
	{
		end = iter;
		if (text[iter] == ' ')
		{
			m_drawCalls.push_back({ text.substr(begin,end - begin),x + (begin * m_width * containsSpaces),y ,colour});
			begin = iter + 1;
			containsSpaces = 1;
		}
	}
	m_drawCalls.push_back({ text.substr(begin,end- begin + 1),x + (begin * m_width * containsSpaces),y ,colour});
}

void Text::display()
{
	for(std::vector<DrawCall>::const_iterator call = m_drawCalls.begin(); call != m_drawCalls.end(); call++)
		for (size_t i = 0; i < call->text.size(); i++)
		{
			const int index = m_indexes[toupper(call->text[i])];
			const float size = m_indexes.size();
			const float uMin = (index ) / size;
			const float uMax = (index + 1) / size;

 			m_app->SetSpriteUVCoordinates(m_spriteId,uMin, 0, uMax, 1);
			m_app->MoveSprite(m_spriteId, call->x + i * (m_width +2), call->y);
			m_app->SetSpriteColour(m_spriteId, call->colour);
			m_app->DrawSprite(m_spriteId);
		}
	m_drawCalls.clear();
}

void Text::initialize()
{
	m_indexes['0'] = 0;
	m_indexes['Z'] = 1;
	m_indexes['2'] = 2;
	m_indexes['3'] = 3;
	m_indexes['4'] = 4;
	m_indexes['5'] = 5;
	m_indexes['6'] = 6;
	m_indexes['7'] = 7;
	m_indexes['8'] = 8;
	m_indexes['9'] = 9;
	m_indexes['A'] = 10;
	m_indexes['B'] = 11;
	m_indexes['C'] = 12;
	m_indexes['D'] = 13;
	m_indexes['E'] = 14;
	m_indexes['F'] = 15;
	m_indexes['G'] = 16;
	m_indexes['H'] = 17;
	m_indexes['I'] = 18;
	m_indexes['J'] = 19;
	m_indexes['K'] = 20;
	m_indexes['L'] = 21;
	m_indexes['M'] = 22;
	m_indexes['N'] = 23;
	m_indexes['O'] = 24;
	m_indexes['P'] = 25;
	m_indexes['Q'] = 26;
	m_indexes['R'] = 27;
	m_indexes['S'] = 28;
	m_indexes['T'] = 29;
	m_indexes['U'] = 30;
	m_indexes['V'] = 31;
	m_indexes['W'] = 32;
	m_indexes['X'] = 33;
	m_indexes['Y'] = 34;
	m_indexes['1'] = 35;

	m_spriteId = m_app->CreateSprite("bitmaps/pixel.png", 683, 33);
	m_width = 18;
	m_app->SetSpriteScale(m_spriteId, m_width, 33);
}

Text::DrawCall::DrawCall(const std::string & text, const float x, const float y, const NHTV::SColour colour):
	text(text),x(x),y(y),colour(colour)
{
}
