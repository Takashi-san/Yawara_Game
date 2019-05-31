#pragma once // Alows to initializate the header just once

#include "GameObject.h"
#include "Vec2.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

#define SPEEDC 250

class Camera
{
private:
	static std::weak_ptr<GameObject> focus1, focus2;

public:
	static Vec2 pos;
	static Vec2 speed;
	static float ratio;

	static void Follow(std::weak_ptr<GameObject>);
	static void Follow(std::weak_ptr<GameObject>, std::weak_ptr<GameObject>);
	static void Unfollow();
	static void Update(float);
};