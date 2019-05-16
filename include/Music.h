#define INCLUDE_SDL
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

#ifndef MUSIC
	#define MUSIC

	class Music {
	private:
		std::shared_ptr<Mix_Music> music;

	public:
		Music();
		Music(std::string);
		~Music();
		void Play(int);
		void Stop(int);
		void Open(std::string);
		bool IsOpen();
	};

#endif