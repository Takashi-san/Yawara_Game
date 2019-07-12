#pragma once // Alows to initializate the header just once

#define INCLUDE_SDL
#include "SDL_include.h"
#include "Music.h"
#include "State.h"
#include "TileSet.h"
#include "Sprite.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

class StageState : public State
{
private:
	Music bgMusic;
	TileSet *tileset;

	Sprite* black;

public:
	StageState();
	~StageState();

	void LoadAssets();
	void Update(float dt);
	void Render();

	void Start();
	void Pause();
	void Resume();
};