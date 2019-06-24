#pragma once // Alows to initializate the header just once

#include "Hitbox.h"
#include "GameObject.h"
#include "Vec2.h"
#include "Timer.h"

#include <string>
#include <stdbool.h>
#include <iostream>
#include <memory>

using namespace std;

// class Hitbox;

class Tongue : public Hitbox
{
private:
	Vec2 speed;
	int angle;
	float distanceLeft;
	float maxDistance;
	float radius;
	weak_ptr<GameObject> center;


public:
	bool targetsPlayer;
	Tongue(GameObject &, int, float, int, float, bool);

	void Update(float);
	void NotifyCollision(GameObject &);
};