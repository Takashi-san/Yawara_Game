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

class Yawara : public Component{
private:
	Vec2 speed;
	int hp, att;
	float def;
	bool idle, change_sprite;
	enum Direction { RIGHT, LEFT, DOWN, DOWN_LEFT, DOWN_RIGHT, UP, UP_LEFT, UP_RIGHT };
	enum Action { MOV, ATK, DGE };
	Direction dir;
	Action act;

	std::weak_ptr<GameObject> tapu;

	typedef struct
	{
		bool isBoosted;
		float factor;
	} boosters;
	

	std::unordered_map<int, boosters> boostMap;

	void Comand(float);
	void DoAction(float);
	void SetMov();
	void SetDge();
	void SetAtk();

	Timer dge_cd;
	Timer dge_act;
	Timer atk_cd;
	Timer atk_act;

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
	Vec2 GetCenterPos();

	enum Boosts { HPBOOST, ATTBOOST, DEFBOOST };

	void Boost(Boosts, float);
};