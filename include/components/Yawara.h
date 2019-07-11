#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"
#include "Timer.h"
#include "Howl.h"
#include "Sound.h"
#include "Sprite.h"

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
	enum Action { MOV, ATK, DGE, HOWL };
	Direction dir;
	Action act;

	Timer hitTime;

	int safeX;
	int safeY;

	std::weak_ptr<GameObject> tapu;
	std::weak_ptr<GameObject> shadow_ptr;
	Sprite* shadow;

	typedef struct
	{
		bool isBoosted;
		float factor;
	} boosters;

	std::unordered_map<int, boosters> boostMap;

	Howl* howl;
	Sound* bite;
	Sound* hit_scream;

	bool gotHit;
	bool isDead;

	void Comand(float);
	void DoAction(float);
	void SetMov();
	void SetDge();
	void SetAtk();
	void SetHowl();

	Timer dge_cd;
	Timer dge_act;
	Timer atk_cd;
	Timer atk_act;
	Timer howl_cd;
	Timer howl_act;

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
	int GetHP();
	int GetMaxHP();
	int GetDirection();

	enum Boosts { HPBOOST, ATTBOOST, DEFBOOST };

	void Boost(Boosts, float);
};