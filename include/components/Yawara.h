#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>
#include <unordered_map>
#include <vector>

class Yawara : public Component{
private:
	Vec2 speed;
	int hp, att;
	float def;
	bool idle, change_sprite;
	enum Direction { RIGHT, UP, LEFT, DOWN, LEFT_DOWN, LEFT_UP, RIGHT_DOWN, RIGHT_UP };
	Direction dir;

	std::weak_ptr<GameObject> tapu;

	typedef struct
	{
		bool isBoosted;
		float factor;
	} boosters;
	

	std::unordered_map<int, boosters> boostMap;

public:
	static Yawara* player;
	
	Yawara(GameObject&);
	~Yawara();

	void Start();
	void Update(float);
	void Render();
	bool Is(std::string);

	void NotifyCollision(GameObject&);

	Vec2 GetPos();

	enum Boosts { HPBOOST, ATTBOOST, DEFBOOST };

	void Boost(Boosts, float);
};