#pragma once // Alows to initializate the header just once

#define INCLUDE_SDL
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"
#include "GameObject.h"
#include "Component.h"

#include <string>
#include <iostream>
#include <stdbool.h>

class Sound : public Component
{
private:
	std::shared_ptr<Mix_Chunk> chunk;
	int channel;

public:
	Sound(GameObject &);
	Sound(GameObject &, std::string);
	~Sound();

	void Play(int = 1, int = MIX_MAX_VOLUME/2);
	void Stop();
	void Open(std::string);
	bool IsOpen();

	void FadeOut(int);
	void PlayFadeIn(int, int = MIX_MAX_VOLUME/2, int = 1, int = -1);

	void Update(float);
	void Render();
	bool Is(std::string);
};