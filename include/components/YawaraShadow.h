#pragma once // Alows to initializate the header just once

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"
#include "Sprite.h"
#include "Timer.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class YawaraShadow : public Component
{
private:
	Sprite* sprite;
	enum Direction { RIGHT, LEFT, DOWN, DOWN_LEFT, DOWN_RIGHT, UP, UP_LEFT, UP_RIGHT };
	Direction dir;

public:
	YawaraShadow(GameObject&);

	void Update(float);
	void Start();
	void Render();
	bool Is(std::string);
};