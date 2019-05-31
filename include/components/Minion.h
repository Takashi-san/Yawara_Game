#pragma once // Alows to initializate the header just once

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>
#define _USE_MATH_DEFINES
#include <math.h>

#define MINION_VEL_ANG 0.5
#define MINION_BULLET_SPEED 600
#define MINION_BULLET_DAMAGE 0
#define MINION_BULLET_RANGE 1000

class Minion : public Component
{
private:
	std::weak_ptr<GameObject> alienCenter;
	float arc;
	int hp;

public:
	Minion(GameObject &, std::weak_ptr<GameObject>, float = 0);

	void Update(float);
	void Render();
	bool Is(std::string);
	void NotifyCollision(GameObject &);
	void Shoot(Vec2);
};