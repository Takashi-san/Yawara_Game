#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_TTF
#include "SDL_include.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <unordered_map>
#include <memory>

#ifndef RESOURCES
	#define RESOURCES

	class Resources{
	private:
		static std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> 	imageTable;
		static std::unordered_map<std::string, std::shared_ptr<Mix_Music>> 		musicTable;
		static std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>> 		soundTable;
		static std::unordered_map<std::string, std::shared_ptr<TTF_Font>> 		fontTable;

	public:
		static std::shared_ptr<SDL_Texture> GetImage(std::string);
		static void ClearImages();

		static std::shared_ptr<Mix_Music> GetMusic(std::string);
		static void ClearMusics();

		static std::shared_ptr<Mix_Chunk> GetSound(std::string);
		static void ClearSounds();

		static std::shared_ptr<TTF_Font> GetFont(std::string, int);
		static void ClearFonts();
	};	
#endif