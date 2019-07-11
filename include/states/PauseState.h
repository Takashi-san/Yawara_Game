#pragma once // Alows to initializate the header just once

#include "State.h"
#include "Sound.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class PauseState : public State
{
private:
	GameObject* selection;
	enum Option { RESUME, QUIT };
	Option opt;

	float xpos[2], ypos;
	Sound *changeSelection, *select, *play;

public:

	PauseState();
	~PauseState();

	void LoadAssets();
	void Update(float dt);
	void Render();

	void Start();
	void Pause();
	void Resume();
};