#include <math.h>

#ifndef VEC2
	#define VEC2

	class Vec2 {
	public:
		float x, y;

		Vec2(){}
		Vec2(float, float);

		void Rotate(float);
		float Modulo();
		Vec2 GetRotated(float);
		Vec2 operator+(const Vec2&);
		Vec2 operator-(const Vec2&);
		Vec2 operator*(const float);
		Vec2 operator/(const float);
	};
#endif