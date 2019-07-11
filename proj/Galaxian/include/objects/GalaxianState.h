#pragma once
#include "Bullet.h"
#include "Curve.h"


class Galaxian;

class GalaxianState
{
public:
	virtual GalaxianState *update(Galaxian *obj, const float elapsedTime) = 0;
};

class GalaxianFormation : public GalaxianState
{
public:
	virtual GalaxianState *update(Galaxian *obj, const float elapsedTime) override final;
};

class GalaxianDive : public GalaxianState
{
public:
	// This constructor decides the duration of the dive
	// The row and column values are passed by a Loop state
	GalaxianDive(Galaxian *obj, size_t row, size_t column, const float diveDuration);

	virtual GalaxianState *update(Galaxian *obj, const float elapsedTime) override final;

protected:

	const size_t m_row, m_column;

	float m_shootTimeline;
	int m_shootCount;

	Curve m_curve;
};

class GalaxianAggressiveDive : public GalaxianDive
{
public:
	GalaxianAggressiveDive(Galaxian *obj, size_t row, size_t column);
};

class GalaxianReturnToFormation : public GalaxianState
{
public:
	// Adds object back to group
	GalaxianReturnToFormation(Galaxian *obj, const size_t row, const size_t column);

	GalaxianState *update(Galaxian *obj, const float elapsedTime)override final;

private:
	
	float m_elapsedTime;
	const float m_timeTillFormation;

	const size_t m_row, m_column;
};

class GalaxianLoop : public GalaxianState
{
public:
	GalaxianLoop(Galaxian *obj);

	GalaxianState * update(Galaxian *obj, const float elapsedTime) override final;
	
protected:

	virtual GalaxianState * getState(Galaxian * obj) = 0;
	size_t m_row, m_column;
	float m_direction;

private:

	Curve m_curve;
};

class GalaxianNormalLoop : public GalaxianLoop
{
public:
	GalaxianNormalLoop(Galaxian * obj);

protected:

	virtual GalaxianState* getState(Galaxian *obj);
};