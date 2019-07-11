#pragma once // Alows to initializate the header just once

#include "GameObject.h"
#include "Vec2.h"
#include "Hitbox.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class Bullet : public Hitbox
{
private:
	Vec2 speed;
	float timeLeft;
	int damage;

public:
	bool targetsPlayer;
	Bullet(GameObject &, float, float, int, float, std::string, int = 1, float = 1, bool = true, bool = true);

	void Update(float);
	void Render();
	bool Is(std::string);
	void NotifyCollision(GameObject &);
	int GetDamage();
};