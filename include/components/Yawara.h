#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class Yawara : public Component{
private:
	Vec2 speed;
	int hp;
	bool idle, change_sprite;
	enum Direction { RIGHT, LEFT, DOWN, DOWN_LEFT, DOWN_RIGHT, UP, UP_LEFT, UP_RIGHT };
	enum Action { MOV, ATK, DGE };
	Direction dir;
	Action act;

	std::weak_ptr<GameObject> tapu;

	void CheckInput();
	void DoAction(float);
	void SetMov();

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
};