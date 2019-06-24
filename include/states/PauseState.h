#pragma once // Alows to initializate the header just once

#include "State.h"

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