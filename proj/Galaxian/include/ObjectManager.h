#pragma once
#include <vector>

class Game;
class Object;

// The Object Manager keeps track of all the objects and updates them
class ObjectManager
{
public:

	Object* push(Object * object);

	void update(const float elapsedTime);

	void clear();

	void draw(Game & app) const;

	// Used for other managers that need to access all the objects
	std::vector<Object*> &getObjects();

	Object* getObjectById(const unsigned int id) const;

private:

	std::vector<Object*> m_objects;
	std::vector<Object*> m_objectsToAdd;
};