#pragma once // Alows to initializate the header just once

#include "Vec2.h"

class Rect
{
public:
	float x, y, w, h;

	bool Within(int, int);
	Vec2 Center();
	void Centered(float, float);
	void Centered(Vec2);
};