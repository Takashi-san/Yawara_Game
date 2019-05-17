#include "Rect.h"

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