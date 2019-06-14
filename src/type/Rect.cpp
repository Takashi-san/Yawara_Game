#include "Rect.h"

Rect::Rect()
{
}

Rect::Rect(float x, float y, float w, float h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

bool Rect::Within(int x, int y) {
	if ((x >= this->x) && (x <= this->x+w) && (y >= this->y) && (y <= this->y+h)) {
		return true;
	} else {
		return false;
	}
}

Vec2 Rect::Center() {
	Vec2 ans(x+w/2, y+h/2);
	return ans;
}

void Rect::Centered(float ref_x, float ref_y) {
	x = ref_x - w/2;
	y = ref_y - h/2;
}

void Rect::Centered(Vec2 ref) {
	Centered(ref.x, ref.y);
}

Rect Rect::operator+(const Vec2 &vector)
{

    return Rect(this->x + vector.x, this->y + vector.y, this->w, this->h);
}

Rect Rect::operator-(const Vec2 &vector)
{

    return Rect(this->x - vector.x, this->y - vector.y, this->w, this->h);
}

Rect Rect::operator*(const Vec2 &vector)
{
    return Rect(this->x, this->y, this->w * vector.x, this->h * vector.y);
}

void Rect::operator+=(const Vec2 &vector)
{

    this->x += vector.x;
    this->y += vector.y;
}

void Rect::operator+=(const Rect &rect)
{

    this->x += rect.x;
    this->y += rect.y;
}

void Rect::operator-=(const Vec2 &vector)
{

    this->x -= vector.x;
    this->y -= vector.y;
}

void Rect::operator=(const Rect &rect)
{

    this->x = rect.x;
    this->y = rect.y;
    this->w = rect.w;
    this->h = rect.h;
}