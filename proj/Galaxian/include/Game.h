#pragma once

#include "Application.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "Starfield.h"
#include "ScoreHandler.h"
#include "Life.h"

// Derived application class that wraps up all globals neatly
class Game : public NHTV::Application
{
public:

	Game();
	virtual ~Game();

	// Basically a glorified singleton pattern but with no public access
	ObjectManager m_objectManager;
	CollisionManager m_collisionManager;
	EventManager m_eventManager;

	// These values represent the borders for the play area
	unsigned int m_minX;
	unsigned int m_maxX;
	unsigned int m_minY;
	unsigned int m_maxY;

protected:

	virtual bool onCreate(int a_argc, char* a_argv[]);
	virtual void onUpdate(float a_deltaTime);
	virtual void onDraw();
	virtual void onDestroy();

private:

	void loadLevel1();
	
	// This timeline keeps track of the Galaxian group and when to let them respawn
	float m_levelTimeline;	
	// This timeline runs when the player is dead to restart
	float m_playerTimeline;
	// This is used for the press enter text
	float m_demoTimeline;
	// This is used when transitioning from demo to play state
	float m_switchTimeline;
	bool m_isPlaying;
	

	int m_iScreenWidth;
	int m_iScreenHeight;

	Starfield m_starfield;
	Life m_lives;
	ScoreHandler m_score;
};