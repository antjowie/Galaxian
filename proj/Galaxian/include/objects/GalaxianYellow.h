#pragma once
#include "objects/Galaxian.h"

class GalaxianYellow:
	public Galaxian
{
public:

	GalaxianYellow(Game &app, const float x, const float y);

	virtual GalaxianState *dive()override final;
};