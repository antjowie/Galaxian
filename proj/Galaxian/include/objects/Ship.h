#pragma once
#include "objects/Object.h"
#include "objects/Bullet.h"

class Command
{
public:

	virtual ~Command();

	virtual void execute(Object *obj, const float elapsedTime) = 0;
};

class PlayerShoot : public Command
{
public:

	virtual ~PlayerShoot();

	virtual void execute(Object *obj, const float elapsedTime) override final;
	
private:

	bool m_isShooting = false;
	Bullet* m_bullet;
};

class PlayerMove : public Command
{
public:

	virtual void execute(Object *obj, const float elapsedTime) override final;

private:
	
	const float m_speed = 150;
};

class DemoShoot : public Command
{
public:

	virtual ~DemoShoot();

	virtual void execute(Object *obj, const float elapsedTime) override final;

private:

	Bullet* m_bullet;
};

class DemoMove : public Command
{
public:

	virtual void execute(Object *obj, const float elapsedTime) override final;

private:

	const float m_speed = 150;
	const float m_switchMoveTime = 0.2f;
	float m_elapsedTime = 0.f;

	// The direction this object is moving in right now
	int m_currentMove = 0;
};

//----------------------------

class Ship : public Object
{
public:

	// Position is from the center
	Ship(Game & app, const float x, const float y);
	virtual ~Ship();

	virtual void update(const float elapsedTime) override final;
	virtual void onEvent(const Event event, Object* obj) override final;

	bool m_bulletShot;

	unsigned int m_bulletId;

protected:

	Command* m_moveset;
	Command* m_shoot;
};

class DemoShip : public Ship
{
public:
	DemoShip(Game &app, const float x, const float y);
};