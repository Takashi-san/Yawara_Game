#define INCLUDE_SDL
#include "SDL_include.h"
#include "Music.h"
#include "State.h"
#include "TileSet.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

#ifndef STAGESTATE
	#define STAGESTATE

	class StageState : public State{
	private:
		Music bgMusic;
		TileSet* tileset;
		
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

#endif