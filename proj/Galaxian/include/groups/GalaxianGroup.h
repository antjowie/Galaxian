#pragma once
#include "groups/Group.h"

class GalaxianGroup;

// We can easily change attack patterns this way
class AttackPattern
{
public:
	virtual void execute(GalaxianGroup *group, const float elapsedTime) =0;
};

class GalaxianGroup :
	public Group
{
public:

	GalaxianGroup(Game &app, const float x, const float y);

	virtual void update(const float elapsedTime) override final;

	virtual void getPosition(const unsigned int id, float &x, float &y) const;

private:

	AttackPattern* m_pattern;

	// 1 - Blue Galaxian
	// 2 - Purple Galaxian
	// 3 - Red Galaxian
	// 4 - Commander Galaxian
	virtual Object* numberToObject(const unsigned short number) const override final;

	float m_elapsedTime;

	// The wobbleMovement of the objects
	// The wobble consists of four states
	float m_wobbleTimeline[10];
	const float m_wobbleDuration;

	bool m_movingRight;
	const float m_speed;
};

//------------------------
// This attack pattern launches 3 galaxians 
// with an interval of 1 seconds inbetween
// every 6 seconds
class NormalAttack : public AttackPattern
{
public:
	NormalAttack();

	virtual void execute(GalaxianGroup * group, const float elapsedTime) override final;

private:
	float m_attackInterval;
	int m_attackCount;

	float m_attackTimeline;
};

class MadAttack : public AttackPattern
{
public:
	MadAttack();

	virtual void execute(GalaxianGroup * group, const float elapsedTime) override final;

private:
	float m_attackInterval;
	int m_attackCount;

	float m_attackTimeline;
};