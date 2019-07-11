#include "ScoreHandler.h"
#include "Text.h"
#include "objects/Galaxian.h"

ScoreHandler::ScoreHandler() :
	m_score(0),
	m_up(0)
{
}

void ScoreHandler::onEvent(Event event, Object * obj)
{
	switch (event)
	{
	case Galaxian_Destroyed:
		m_score += static_cast<Galaxian*>(obj)->m_score;
		m_up	+= static_cast<Galaxian*>(obj)->m_score;
		break;
	}
}

void ScoreHandler::draw()
{
	const float m_iScreenWidth = 1280;
	const float m_iScreenHeight = 720;
	Text::draw("High score", m_iScreenWidth* 0.05f, m_iScreenHeight * 0.75f);
	Text::draw(std::to_string(m_score), m_iScreenWidth* 0.05f, m_iScreenHeight * 0.70f, { 255,0,0,255 });
	Text::draw("1Up", m_iScreenWidth* 0.05f, m_iScreenHeight * 0.6f);
	Text::draw(std::to_string(m_up), m_iScreenWidth* 0.05f, m_iScreenHeight * 0.55f, { 255,0,0,255 });
}

void ScoreHandler::resetScore()
{
	m_score = 0;
	m_up = 0;
}
