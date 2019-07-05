#pragma once // Alows to initializate the header just once

#include "Hitbox.h"
#include "GameObject.h"
#include "Vec2.h"
#include "Timer.h"

#include <string>
#include <stdbool.h>
#include <iostream>
#include <memory>

class Claw : public Hitbox
{
private:
	Timer duration;
	int damage;

public:
	bool targetsPlayer;
	Claw(GameObject &, int, bool);

	void Update(float);
	void NotifyCollision(GameObject &);
};