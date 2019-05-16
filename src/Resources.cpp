#include "Resources.h"
#include "Game.h"

std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> 	Resources::imageTable;
std::unordered_map<std::string, std::shared_ptr<Mix_Music>> 	Resources::musicTable;
std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>> 	Resources::soundTable;
std::unordered_map<std::string, std::shared_ptr<TTF_Font>> 		Resources::fontTable;

std::shared_ptr<SDL_Texture> Resources::GetImage(std::string file) {
	Game& instance = Game::GetInstance();
	SDL_Texture* texture;
	std::unordered_map<std::string, std::shared_ptr<SDL_Texture>>::iterator it = imageTable.find(file.c_str());

	if (it != imageTable.end()){
		return it->second;
	}

	texture = IMG_LoadTexture(instance.GetRenderer(), file.c_str());
	if (texture != nullptr) {
		std::shared_ptr<SDL_Texture> s_txtr(texture, [](SDL_Texture* texture){ SDL_DestroyTexture(texture); });
		imageTable[file.c_str()] = s_txtr;
		return imageTable[file.c_str()];
	} else {
		return nullptr;
	}
}

void Resources::ClearImages() {
	std::unordered_map<std::string, std::shared_ptr<SDL_Texture>>::iterator it;

	it = imageTable.begin();
	while (it != imageTable.end()) {
		if (it->second.unique()) {
			it = imageTable.erase(it);
		} else {
			it++;
		}
	}
}

std::shared_ptr<Mix_Music> Resources::GetMusic(std::string file) {
	Mix_Music* music;
	std::unordered_map<std::string, std::shared_ptr<Mix_Music>>::iterator it = musicTable.find(file.c_str());

	if (it != musicTable.end()){
		return it->second;
	}

	music = Mix_LoadMUS(file.c_str());
	if (music != nullptr) {
		std::shared_ptr<Mix_Music> s_music(music, [](Mix_Music* music){ Mix_FreeMusic(music); });
		musicTable[file.c_str()] = s_music;
		return musicTable[file.c_str()];
	} else {
		return nullptr;
	}
}

void Resources::ClearMusics() {
	std::unordered_map<std::string, std::shared_ptr<Mix_Music>>::iterator it;

	it = musicTable.begin();
	while (it != musicTable.end()) {
		if (it->second.unique()) {
			it = musicTable.erase(it);
		} else {
			it++;
		}
	}
}

std::shared_ptr<Mix_Chunk> Resources::GetSound(std::string file) {
	Mix_Chunk* sound;
	std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>>::iterator it = soundTable.find(file.c_str());

	if (it != soundTable.end()){
		return it->second;
	}

	sound = Mix_LoadWAV(file.c_str());
	if (sound != nullptr) {
		std::shared_ptr<Mix_Chunk> s_sound(sound, [](Mix_Chunk* sound){ Mix_FreeChunk(sound); });
		soundTable[file.c_str()] = s_sound;
		return soundTable[file.c_str()];
	} else {
		return nullptr;
	}
}

void Resources::ClearSounds() {
	std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>>::iterator it;

	it = soundTable.begin();
	while (it != soundTable.end()) {
		if (it->second.unique()) {
			it = soundTable.erase(it);
		} else {
			it++;
		}
	}
}

std::shared_ptr<TTF_Font> Resources::GetFont(std::string file, int fontSize) {
	TTF_Font* font;
	std::string key = file + std::to_string(fontSize);
	std::unordered_map<std::string, std::shared_ptr<TTF_Font>>::iterator it = fontTable.find(key.c_str());

	if (it != fontTable.end()){
		return it->second;
	}

	font = TTF_OpenFont (file.c_str(), fontSize);
	if (font != nullptr) {
		std::shared_ptr<TTF_Font> s_font(font, [](TTF_Font* font){ TTF_CloseFont(font); });
		fontTable[key.c_str()] = s_font;
		return fontTable[key.c_str()];
	} else {
		return nullptr;
	}
}

void Resources::ClearFonts() {
	std::unordered_map<std::string, std::shared_ptr<TTF_Font>>::iterator it;

	it = fontTable.begin();
	while (it != fontTable.end()) {
		if (it->second.unique()) {
			it = fontTable.erase(it);
		} else {
			it++;
		}
	}
}