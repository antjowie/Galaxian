#pragma once
#include "Object.h"

class Box :public Object
{
public:
	Box(Game& app, const float minX, const float minY, const float maxX, const float maxY, NHTV::SColour color);
};