#include "Vec2.h"

#ifndef RECT
	#define RECT
	class Rect {
	public:
		float x, y, w, h;

		bool Within(int, int);
		Vec2 Center();
		void Centered(float, float);
		void Centered(Vec2);
	};
#endif