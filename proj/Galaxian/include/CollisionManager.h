#pragma once

class Game;
class Object;

// The collision manager keeps track of all the collisions and pushes collision
// events to the event manager
class CollisionManager
{
public:

	CollisionManager(Game &app);
	
	void resolveCollision();

private:

	bool intersect(const Object* const obj1, const Object* const obj2) const;

	Game &m_app;
};