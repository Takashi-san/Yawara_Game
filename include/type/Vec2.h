#pragma once // Alows to initializate the header just once

#include <math.h>

#define PI 3.1415926

class Vec2
{
public:
	float x, y;

	Vec2() {}
	Vec2(float, float);

	void Rotate(float);
	float Modulo();
	float Inclination();

	Vec2 GetRotated(float);
	Vec2 operator+(const Vec2 &);
	Vec2 operator-(const Vec2 &);
	Vec2 operator*(const float);
	Vec2 operator/(const float);
	Vec2 operator-=(const Vec2 &);
	bool operator==(const Vec2 &);
	// Vec2 operator+(const Rect &);
};