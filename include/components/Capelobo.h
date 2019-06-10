#pragma once // Alows to initializate the header just once

#include "Component.h"
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

#define ALIEN_VEL_ANG 0
#define ALIEN_SPEED 400
#define ALIEN_REST_BASE 0.5
#define BOSS_MOVEMENT 1.5

#define SAFE_UP 16
#define SAFE_DOWN 142
#define SAFE_SIDE 128

#define CLAW_DAMEGE 20

class Capelobo : public Component
{
private:
	enum CapeloboState
	{
		MOVING,
		RESTING,
		SLEEPING,
		BASIC_ATTACK,
		LOAD_ATTACK
	};
	CapeloboState state;

	enum Direction
	{
		RIGHT,
		UP,
		LEFT,
		DOWN,
		LEFT_DOWN,
		LEFT_UP,
		RIGHT_DOWN,
		RIGHT_UP
	};
	Direction dir;

	Timer restTimer;
	Timer moveTimer;
	float restOffset;
	Vec2 enemyPos;

	Vec2 speed;
	int hp;

	int safeX;
	int safeY;

	bool change_sprite;

public:
	static Capelobo *boss;

	Capelobo(GameObject &, float = 0);
	~Capelobo();

	void Start();
	void Update(float);
	void Render();
	bool Is(std::string);
	void NotifyCollision(GameObject &);
};