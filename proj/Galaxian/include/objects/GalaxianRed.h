#pragma once
#include "objects/Galaxian.h"

class GalaxianRed :
	public Galaxian
{
public:

	GalaxianRed(Game &app, const float x, const float y);

	virtual GalaxianState *dive() override final;
};