#pragma once // Alows to initializate the header just once

#include "Vec2.h"

class Rect
{
public:
	float x, y, w, h;

	Rect();
	Rect(float x, float y, float w, float h);

	bool Within(int, int);
	Vec2 Center();
	void Centered(float, float);
	void Centered(Vec2);
	Rect operator-(const Vec2 &vector);
	Rect operator+(const Vec2 &vector);
	Rect operator*(const Vec2 &vector);
	void operator+=(const Vec2 &vector);
	void operator+=(const Rect &rect);
	void operator-=(const Vec2 &vector);
	void operator=(const Rect &rect);
};