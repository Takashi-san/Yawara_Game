#pragma once // Alows to initializate the header just once

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class Claw : public Component
{
private:
	Vec2 speed;
	float distanceLeft;
	int damage;
	Vec2 center;

public:
	bool targetsPlayer;
	Claw(GameObject &, float, float, int, Vec2, float, float, float, bool);

	void Update(float);
	void Render();
	bool Is(std::string);
	void NotifyCollision(GameObject &);
	int GetDamage();
};