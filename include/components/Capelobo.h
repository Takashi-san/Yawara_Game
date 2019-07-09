#pragma once // Alows to initializate the header just once

#include "Enemy.h"
#include "GameObject.h"
#include "Vec2.h"
#include "State.h"
#include "Timer.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>
#include <vector>
#include <queue>
#include <cmath>

#define BOSS_VEL_ANG 0
#define BOSS_SPEED 350
#define BOSS_REST_BASE 0.5
#define BOSS_MOVEMENT 1

#define CLAW_DAMAGE 10
#define TONGUE_DAMAGE 50
#define TONGUE_SPEED 800
#define TONGUE_MAX_DIST 400

class Capelobo : public Enemy
{
private:
	
	float restOffset;

	Vec2 speed;
	int hp;

	int safeX;
	int safeY;

	bool change_sprite;

public:
	static Capelobo *boss;

	Capelobo(GameObject &, float = 0);
	~Capelobo();

	void Start() override;
	void Update(float) override;
	void Render() override;
	bool Is(std::string);
	void NotifyCollision(GameObject &);
};