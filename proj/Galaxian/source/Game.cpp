#include "Game.h"

#include "objects/Ship.h"
#include "objects/Galaxian.h"
#include "objects/Box.h"

#include "groups/GalaxianGroup.h"
#include "Text.h"

APPLICATION_ENTRY(Game);

Game::Game():
	m_collisionManager(*this),
	m_eventManager(m_objectManager.getObjects()),
	m_levelTimeline(0),
	m_playerTimeline(0),
	m_switchTimeline(0),
	m_demoTimeline(0),
	m_starfield(*this),
	m_isPlaying(false),
	m_lives(*this)
{
}

Game::~Game()
{

}

bool Game::onCreate(int a_argc, char* a_argv[])
{
	// Initialization
	Text::m_app = this;
	Text::initialize();
	m_eventManager.addObserver(&m_score);

	setBackgroundColor(NHTV::SColour(0x00, 0x00, 0x00, 0xFF));
	GetScreenSize(m_iScreenWidth, m_iScreenHeight);
	
	m_minX = static_cast<unsigned> (m_iScreenWidth * 0.25f);
	m_maxX = static_cast<unsigned> (m_iScreenWidth * 0.75f);
	
	m_minY = 0;
	m_maxY = m_iScreenHeight;

	std::srand(static_cast<unsigned>(time(0)));
	std::rand();

	//------------------------------------
	// Spawn
	m_starfield.loadStars();


	const int colour = 20;
	new Box(*this, 0, 0, m_minX, m_maxY, NHTV::SColour(colour, colour, colour, 0xFF));
	new Box(*this, m_maxX, m_minY, m_iScreenWidth, m_iScreenHeight, NHTV::SColour(colour, colour, colour, 0xFF));

	Galaxian::m_group = new GalaxianGroup(*this, m_iScreenWidth * 0.5f, m_iScreenHeight *0.75f);
	
	Galaxian::m_group->setCellSize(42.f, 32.f);

	Object* ship = new DemoShip(*this, m_iScreenWidth * 0.5f, m_iScreenHeight* 0.1f);

	Galaxian::m_playerId = ship->m_id;
	loadLevel1();

	m_lives.setDrawPosition(m_iScreenWidth* 0.1f, m_iScreenHeight * 0.075f);
	m_lives.resetLife();

	return true;
}

void Game::onUpdate(float a_deltaTime)
{
	if (a_deltaTime > 1.f)
		return;

	// Quit our application when escape is pressed
	if ( IsKeyDown(NHTV::KEY_ESCAPE) )
		destroy();

	m_starfield.update(a_deltaTime);                
	Galaxian::m_group->update(a_deltaTime);
	m_objectManager.update(a_deltaTime);
	m_collisionManager.resolveCollision();

	if (!m_isPlaying)
	{
		// Let text flicker
		if (m_switchTimeline == 0)
		{
			m_demoTimeline += a_deltaTime;
			if ((int)std::fmod(m_demoTimeline, 2) == 0)
			{
				//Text::draw("press", 400, m_maxY *0.5f);
				//Text::draw("enter", 500, m_maxY *0.5f);
				//Text::draw("to",	500, m_maxY *0.5f);
				//Text::draw("start", 500, m_maxY *0.5f);
				Text::draw("Press enter to start", m_iScreenWidth * 0.35f, m_maxY *0.5f, { 0,255,255,255 });
			}

			if (IsKeyDown(NHTV::KEY_ENTER))
			{
				m_switchTimeline = 3;
				m_isPlaying = true;
				m_objectManager.clear();

				m_starfield.loadStars();

				const int colour = 20;
				new Box(*this, 0, 0, m_minX, m_maxY, NHTV::SColour(colour, colour, colour, 0xFF));
				new Box(*this, m_maxX, m_minY, m_iScreenWidth, m_iScreenHeight, NHTV::SColour(colour, colour, colour, 0xFF));
			}
		}
	}

	if (m_isPlaying)
	{
		m_switchTimeline -= a_deltaTime;
		if (m_switchTimeline< 0.f)
		{
			m_switchTimeline = -1;
			m_isPlaying = false;
			Galaxian::m_playerId = (new Ship(*this, m_iScreenWidth * 0.5f, m_iScreenHeight* 0.1f))->m_id;

			Galaxian::m_group->moveToOriginal();
			m_lives.resetLife();
			m_score.resetScore();
			loadLevel1();
		}
	}

 	// Check if all Galaxians are dead
	if (Galaxian::m_group->getCount() == 0)
	{
		m_levelTimeline += a_deltaTime;
		// The player has wons
		if (m_levelTimeline > 10.f)
		{
			std::vector<Object*> objects = m_objectManager.getObjects();
			if (std::count_if(objects.begin(), objects.end(), [](Object*obj) {return obj->m_name == "galaxians"; }) == 0)
			{
				Galaxian::m_group->moveToOriginal();
				loadLevel1();
			}
		}
	}
	else
		m_levelTimeline = 0;

	std::vector<Object*> objects = m_objectManager.getObjects(); 
	int count = std::count_if(objects.begin(), objects.end(), [](Object *obj) {return obj->m_name == "ship"; });
	if (std::count_if(objects.begin(), objects.end(), [](Object *obj) {return obj->m_name == "ship"; }) == 0)
	{
		m_playerTimeline += a_deltaTime;
		if (m_playerTimeline > 5.f)
		{
			if(m_switchTimeline == -1)
				Galaxian::m_playerId = (new Ship(*this, m_iScreenWidth * 0.5f, m_iScreenHeight* 0.1f))->m_id;
			else
				Galaxian::m_playerId = (new DemoShip(*this, m_iScreenWidth * 0.5f, m_iScreenHeight* 0.1f))->m_id;
			
			if (m_lives.getLifeCount() == 0)
			{
				Galaxian::m_group->moveToOriginal();
				m_lives.resetLife();
				m_score.resetScore();
				loadLevel1();
			}
			else
			m_lives.popLife();
		}
	}
	else
		m_playerTimeline = 0;

	//if (IsKeyDown(NHTV::KEY_P))
	//	loadLevel1();
}

void Game::onDraw()
{
	clearScreen();

	m_starfield.draw();
	m_objectManager.draw(*this);

	//------------------
	// All hud elements
	for (std::vector<Object*>::const_iterator hud = m_objectManager.getObjects().begin(); hud != m_objectManager.getObjects().end(); hud++)
		if ((*hud)->m_name == "hud")
			DrawSprite((*hud)->m_id);

	m_lives.draw();
	m_score.draw();

	Text::display();

	/*DrawLine(0, m_minY , m_minX, m_maxY, NHTV::SColour(20, 20, 20, 0xFF));
	DrawLine(m_maxX, m_minY , m_iScreenWidth, m_maxY, NHTV::SColour(20, 20, 20, 0xFF));
*/
	//float fDeltaT = GetDeltaTime();
	
	//DrawString("SCORE < 1 >", m_iScreenWidth * 0.025f, m_iScreenHeight - 2, 0.5f);
	//DrawString("HI-SCORE", m_iScreenWidth * 0.4f, m_iScreenHeight - 2, 0.5f);

	/*DrawString("SCORE < 2 >", m_iScreenWidth * 0.725f, m_iScreenHeight - 2, 0.5f);
	DrawString("CREDIT", m_iScreenWidth * 0.65f, 38);
*/
//	DrawLine(0, 40, m_iScreenWidth, 40, NHTV::SColour(0x00, 0xFC, 0x00, 0xFF));
//	std::string timeFrame = "DeltaTime: ";
//	timeFrame.append(std::to_string(fDeltaT));
//	DrawString(timeFrame.c_str(), 10, 720, 0.75f);
}

void Game::onDestroy()
{
	
}

void Game::loadLevel1()
{
	// Clear all galaxians
	std::vector<Object*> galaxians = m_objectManager.getObjects();
	for (int i = 0; i < galaxians.size(); i++)
		if(galaxians[i]->m_name == "galaxian")
			galaxians[i]->destroy();

	unsigned int level[6][10] =
	{
		{0,0,0,4,0,0,4,0,0,0},
		{0,0,3,3,3,3,3,3,0,0},
		{0,2,2,2,2,2,2,2,2,0},
		{1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1}
	};
	Galaxian::m_group->loadGroup(level);
}