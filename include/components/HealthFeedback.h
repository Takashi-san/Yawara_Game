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

class HealthFeedback : public Component
{
private:
	Sprite* sprite;
	Timer ease_time;
	bool cicle;

public:
	HealthFeedback(GameObject&);

	void Update(float);
	void Start();
	void Render();
	bool Is(std::string);
};