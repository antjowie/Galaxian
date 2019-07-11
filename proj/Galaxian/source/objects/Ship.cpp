#include "objects/Explosion.h"
#include "objects/Ship.h"
#include "objects/Galaxian.h"

Ship::Ship(Game & app,const float x, const float y):
	Object(app,"ship","./images/shooter.png",x,y,32,30,32,32),
	m_moveset(new PlayerMove()),
	m_shoot	 (new PlayerShoot()),
	m_bulletShot(false),
	m_bulletId(0)
{
}

Ship::~Ship()
{
	if (m_shoot) delete m_shoot;
	if (m_moveset) delete m_moveset;
}

void Ship::update(const float elapsedTime)
{
	m_moveset->execute(this,elapsedTime);
	m_shoot->execute(this,elapsedTime);
}

void Ship::onEvent(const Event event, Object * obj)
{
	switch (event)
	{
	case Collision:
		// Spawn explosion
		float x, y;
		m_app.GetSpritePosition(m_id, x, y);
		new Explosion(m_app, x, y,true);
		Galaxian::m_playerId = 0;
		destroy();

		break;
	case Bullet_Destroyed:
		if(obj->m_id == m_bulletId)
			m_bulletShot = false;
		break;
	}
}

void PlayerMove::execute(Object *obj, const float elapsedTime)
{
	Ship* ship = static_cast<Ship*>(obj);

	float xPos = 0; float yPos = 0;
	obj->m_app.GetSpritePosition(obj->m_id, xPos, yPos);

	if (obj->m_app.IsKeyDown(NHTV::KEY_LEFT))
		xPos -= m_speed * elapsedTime;

	if (obj->m_app.IsKeyDown(NHTV::KEY_RIGHT))
		xPos += m_speed * elapsedTime;

	xPos = NHTV::Clampf(xPos, obj->m_app.m_minX + obj->m_width * 0.5f, obj->m_app.m_maxX - obj->m_width * 0.5f);
	obj->m_app.MoveSprite(obj->m_id, xPos, yPos);
}

PlayerShoot::~PlayerShoot()
{
	if(m_bullet) m_bullet->destroy();
}

void PlayerShoot::execute(Object *obj, const float elapsedTime)
{
	Ship* ship = static_cast<Ship*>(obj);
	if (ship->m_bulletShot) return;

	float xPos = 0; float yPos = 0;
	obj->m_app.GetSpritePosition(obj->m_id, xPos, yPos);

	// Spawn a new bullet if bullet is gone
	if (m_bullet == nullptr)
		m_bullet = new Bullet(obj->m_app, xPos, yPos + 18,600);
	else
		obj->m_app.MoveSprite(m_bullet->m_id, xPos, yPos + 18);

	if (obj->m_app.IsKeyDown(NHTV::KEY_S))
	{
		// Makes sure that player can't keep holding shoot key
		if (m_isShooting == false)
		{
			m_bullet->shoot(xPos,obj->m_app.m_maxY*2.f);
			ship->m_bulletId = m_bullet->m_id;
			m_bullet = nullptr;
			ship->m_bulletShot = true;
		}
		m_isShooting = true;
	}
	else
		m_isShooting = false;
}

void DemoMove::execute(Object *obj, const float elapsedTime)
{
	Ship *ship = static_cast<Ship*>(obj);
	m_elapsedTime += elapsedTime;

	if (m_elapsedTime > m_switchMoveTime)
	{
		m_elapsedTime = std::fmod(m_elapsedTime, m_switchMoveTime);
		m_currentMove = std::rand() % 3 - 1;
	}
	
	float xPos = 0; float yPos = 0;
	obj->m_app.GetSpritePosition(obj->m_id, xPos, yPos);

	xPos += m_currentMove * m_speed * elapsedTime;

	xPos = NHTV::Clampf(xPos, obj->m_app.m_minX + obj->m_width * 0.5f, obj->m_app.m_maxX - obj->m_width * 0.5f);
	obj->m_app.MoveSprite(obj->m_id, xPos, yPos);
}

DemoShoot::~DemoShoot()
{
	if(m_bullet) m_bullet->destroy();
}

void DemoShoot::execute(Object * obj, const float elapsedTime)
{
	Ship* ship = static_cast<Ship*>(obj);
	if (ship->m_bulletShot) return;

	float xPos = 0; float yPos = 0;
	obj->m_app.GetSpritePosition(obj->m_id, xPos, yPos);

	// Spawn a new bullet if bullet is gone
	if (m_bullet == nullptr)
		m_bullet = new Bullet(obj->m_app, xPos, yPos + 18,600);
	else
		obj->m_app.MoveSprite(m_bullet->m_id, xPos, yPos + 18);

	if(std::rand() % 25 < 1)
	{
		m_bullet->shoot(xPos, obj->m_app.m_maxY*2.f);
		ship->m_bulletId = m_bullet->m_id;
		m_bullet = nullptr;
		ship->m_bulletShot = true;
	}
}

Command::~Command()
{
}

DemoShip::DemoShip(Game & app, const float x, const float y):
	Ship(app,x,y)
{
	delete m_moveset;
	delete m_shoot;
	m_moveset = new DemoMove();
	m_shoot = new DemoShoot();
}
