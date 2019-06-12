#pragma once // Alows to initializate the header just once

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"
#include "Timer.h"

#include <string>
#include <stdbool.h>
#include <iostream>
#include <memory>

using namespace std;

class Claw : public Component
{
private:
	float speed;
	float angleLeft;
	float atAngle;
	float radius;
	Timer duration;
	int damage;
	weak_ptr<GameObject> center;

public:
	bool targetsPlayer;
	Claw(GameObject &, float, float, int, weak_ptr<GameObject>, float, float, float, bool);

	void Update(float);
	void Render();
	bool Is(std::string);
	void NotifyCollision(GameObject &);
	int GetDamage();
};