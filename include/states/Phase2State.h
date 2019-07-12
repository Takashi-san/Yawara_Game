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

class Phase2State : public State
{
private:
	Music bgMusic;
	TileSet *tileset;

	Sprite* black;
	Sprite* white;

	bool ignore, flag;
public:
	Phase2State();
	~Phase2State();

	void LoadAssets();
	void Update(float dt);
	void Render();

	void Start();
	void Pause();
	void Resume();
};