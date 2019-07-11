#pragma once // Alows to initializate the header just once

#include "State.h"
#include "Music.h"
#include "Sound.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class EndState : public State
{
public:
	EndState();
	~EndState();

	void LoadAssets();
	void Update(float dt);
	void Render();

	void Start();
	void Pause();
	void Resume();

private:
	Music bgMusic;
	GameObject* selection;

	enum Option {NEWGAME, QUIT};
	Option Opt;

	float xpos[2], ypos;
	Sound *changeSelection, *select, *play;
};