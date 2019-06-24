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
public:
	Collider* colisor;
	bool targetsPlayer;
	Hitbox(GameObject&, int = 10, bool = false, float = 0);

	void Update(float);
	void Render();
	bool Is(std::string);
	
	int GetDamage();
	void NotifyCollision(GameObject &);
};