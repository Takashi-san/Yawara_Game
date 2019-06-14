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

class Tongue : public Component
{
private:
	Vec2 speed;
	int angle;
	float distanceLeft;
	float maxDistance;
	float radius;
	int damage;
	weak_ptr<GameObject> center;


public:
	bool targetsPlayer;
	Tongue(GameObject &, int, float, int, float, bool);

	void Update(float);
	void Render();
	bool Is(std::string);
	void NotifyCollision(GameObject &);
	int GetDamage();
};