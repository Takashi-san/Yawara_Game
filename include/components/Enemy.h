#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"
#include "Timer.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>
#include <unordered_map>
#include <vector>

#define SAFE_DISTANCE_UP 16 * 0.7
#define SAFE_DISTANCE_DOWN 48 * 0.7
#define HIT_COOL_DOWN 1

class Enemy : public Component{
protected:
	Vec2 speed;
	Vec2 yawaraPos;

	int hp;
	bool change_sprite;
	bool moveAllowed;

	enum Direction { RIGHT, RIGHT_UP, UP, LEFT_UP, LEFT, LEFT_DOWN, DOWN, RIGHT_DOWN};
	enum Action { MOVING, RESTING, SLEEPING, BASIC_ATTACK, LOAD_ATTACK };
	Direction dir;
	Action state;

	Timer restTimer;
	Timer moveTimer;
	Timer attackTimer;
	Timer hitTimer;

	int safeX;
	int safeY;
	Vec2 enemyPos;

public:
	
	Enemy(GameObject&);
	~Enemy();

	void Start();
	void Update(float);
	void Render();
	bool Is(std::string);

	void NotifyCollision(GameObject&);

	bool AllowedToMove(Vec2);
	void Move45(Vec2&);
};