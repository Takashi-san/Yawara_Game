#pragma once // Alows to initializate the header just once

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"
#include "Sprite.h"
#include "Timer.h"
#include "Sound.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class Howl : public Component
{
private:
	Sprite* sprite;
	Sound*	sound;

	bool exec;
	float sec2Activate, activeTime;
	Timer toActivate;
public:
	Howl(GameObject&);

	void Update(float);
	void Start();
	void Render();
	bool Is(std::string);

	void SetHowl(float, float);
};