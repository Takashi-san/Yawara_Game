#pragma once // Alows to initializate the header just once

#define INCLUDE_SDL
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class Music
{
private:
	std::shared_ptr<Mix_Music> music;

	int volume;

public:
	Music();
	Music(std::string);
	~Music();
	void Play(int = -1, int = 50);
	void Stop(int = 1500);
	void Open(std::string);
	bool IsOpen();

	void SetVolume(int);
	int GetVolume();
};