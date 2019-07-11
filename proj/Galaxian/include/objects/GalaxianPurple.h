#pragma once
#include "objects/Galaxian.h"

class GalaxianPurple:
	public Galaxian
{
public:

	GalaxianPurple(Game &app, const float x, const float y);
	
	virtual GalaxianState *dive() override final;
};