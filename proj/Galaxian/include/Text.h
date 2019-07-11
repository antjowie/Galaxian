#pragma once
#include <unordered_map>
#include <string>

#include "Application.h"

class Game;

class Text
{
public:

	// Every text will be drawn after everything has been drawn
	// so we have to keep the draw call somewhere
	static void draw(const std::string &text, const float x, const float y,const NHTV::SColour colour = { 255,255,255,255 });
	static void display();

	static void initialize();

	static Game *m_app;

private:

	struct DrawCall
	{
		DrawCall(const std::string & text, const float x, const float y,const NHTV::SColour colour);

		const std::string text;
		const float x;
		const float y;
		const NHTV::SColour colour;
	};

	// This map converts characters to indexes
	static std::unordered_map<char, unsigned int>	m_indexes;
	static std::vector<DrawCall>					m_drawCalls;
	
	static unsigned int m_spriteId;
	static unsigned int	m_width;

};
