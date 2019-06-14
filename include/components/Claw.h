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
	Timer duration;
	int damage;

public:
	bool targetsPlayer;
	Claw(GameObject &, int, bool);

	void Update(float);
	void Render();
	bool Is(std::string);
	void NotifyCollision(GameObject &);
	int GetDamage();
};