#pragma once
#include "objects/Galaxian.h"

class GalaxianBlue :
	public Galaxian
{
public:
	GalaxianBlue(Game &app, const float x, const float y);

	virtual GalaxianState* dive() override final;
};

class GalaxianBlueLoop : public GalaxianLoop
{
public:
	GalaxianBlueLoop(Galaxian * obj);

protected:
	virtual GalaxianState* getState(Galaxian * obj);
};

class GalaxianNormalDive : public GalaxianDive
{
public:
	GalaxianNormalDive(Galaxian *obj, size_t row, size_t column);
};
