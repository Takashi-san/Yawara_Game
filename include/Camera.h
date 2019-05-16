#include "GameObject.h"
#include "Vec2.h"

#include <string>
#include <iostream>
#include <stdbool.h>

#ifndef CAMERA
	#define CAMERA

	#define SPEEDC 250

	class Camera {
	private:
		static GameObject* focus;

	public:
		static Vec2 pos;
		static Vec2 speed;

		static void Follow(GameObject*);
		static void Unfollow();
		static void Update(float);
	};

#endif