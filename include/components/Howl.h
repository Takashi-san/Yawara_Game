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

class Howl : public Component
{
private:
	Sprite* sprite;
	Timer toActivate;

	bool exec, cicle;
	float sec2Activate, activeTime;
public:
	Howl(GameObject&);

	void Update(float);
	void Start();
	void Render();
	bool Is(std::string);

	void SetHowl(float, float);
};