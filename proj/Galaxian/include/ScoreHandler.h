#pragma once
#include "EventManager.h"

class ScoreHandler : public Observer
{
public:
	
	ScoreHandler();

	virtual void onEvent(Event event, Object* obj)override final;

	void draw();
	void resetScore();

private:

	unsigned int m_score;
	unsigned int m_up;
};