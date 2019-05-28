#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

#define YAWARA_SPEED 500

class Yawara : public Component{
private:
	Vec2 speed;
	int hp;
	bool idle;
	enum Direction { RIGHT, UP, LEFT, DOWN, LEFT_DOWN, LEFT_UP, RIGHT_DOWN, RIGHT_UP };
	Direction dir, new_dir;

	std::weak_ptr<GameObject> tapu;

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