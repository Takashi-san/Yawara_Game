#pragma once // Alows to initializate the header just once

#include "Component.h"
#include "GameObject.h"
#include "Collider.h"
#include "Timer.h"

#include <cstring>
#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class Hitbox : public Component
{
private:
	int damage;

	float selfDestruct;
	Timer SFTimer;

public:
	Collider* colisor;
	bool targetsPlayer;
	bool hitDie;
	Hitbox(GameObject&, int = 0, bool = false, float = 0, bool = true);

	void Update(float);
	void Render();
	bool Is(std::string);
	
	int GetDamage();
	void SetDamage(int);
	void SetSelfDestruct(float);
	void NotifyCollision(GameObject &);
};