#pragma once // Alows to initializate the header just once

#include "GameObject.h"
#include "Vec2.h"
#include "Hitbox.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class Bullet : public Component
{
private:
	Vec2 speed;
	int damage;
	float distanceLeft;
	std::string spriteEnd;

public:
	bool targetsPlayer;
	Bullet(GameObject &, float, float, int, float, std::string, std::string, int = 1, float = 1, bool = true);

	void Update(float);
	void Render();
	bool Is(std::string);
	int GetDamage();
	void NotifyCollision(GameObject &) override;
};