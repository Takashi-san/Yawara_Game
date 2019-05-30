#pragma once // Alows to initializate the header just once

#include "State.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class TitleState : public State
{
private:
	GameObject* selection;
	enum Option { PLAY, QUIT };
	Option opt;

public:

	TitleState();
	~TitleState();

	void LoadAssets();
	void Update(float dt);
	void Render();

	void Start();
	void Pause();
	void Resume();
};