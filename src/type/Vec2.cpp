#include "Vec2.h"

Vec2::Vec2(float x, float y) {
	this->x = x;
	this->y = y;
}

void Vec2::Rotate(float rad){
	// A referência é angulo negativo = rotação antihorária.
	int xn = x*cos(rad) - y*sin(rad);
	int yn = y*cos(rad) + x*sin(rad);

	x = xn;
	y = yn;
}

Vec2 Vec2::GetRotated(float rad){
	Vec2 ans;
	ans.x = x;
	ans.y = y;
	ans.Rotate(rad);
	return ans;
}

float Vec2::Modulo() {
	return sqrt(x*x + y*y);
}

Vec2 Vec2::operator+(const Vec2& arg) {
	return Vec2(x + arg.x, y + arg.y);
}

Vec2 Vec2::operator-(const Vec2& arg) {
	return Vec2(x - arg.x, y - arg.y);
}

Vec2 Vec2::operator*(const float a) {
	return Vec2(a*x, a*y);
}

Vec2 Vec2::operator/(const float a) {
	return Vec2(x/a, y/a);
}