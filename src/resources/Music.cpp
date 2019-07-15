#include "Music.h"
#include "Game.h"
#include "Resources.h"

Music::Music() {
	music = nullptr;
	volume = 50;
}

Music::Music(std::string file) {
	music = nullptr;
	Open(file);
}

void Music::Play(int times, int volume) {
	if (music != nullptr){
		Mix_PlayMusic(music.get(), times);

		if(volume >= 0 && volume <= 100)
			this->volume = volume;

		Mix_VolumeMusic((this->volume/100.0) * MIX_MAX_VOLUME);
	} else {
		std::cout << "Nenhuma musica carregada para tocar.\n";
	}
}

void Music::Stop(int msToStop) {
	Mix_FadeOutMusic(msToStop);
}

void Music::Open(std::string file) {
	music = Resources::GetMusic(file.c_str());
	if (music == nullptr) {
		std::cout << "Falha em carregar musica: " << file.c_str() << "\n";
		std::cout << "SDL_GetError: " << SDL_GetError() << "\n";
		//exit(EXIT_FAILURE);
	}
}

bool Music::IsOpen() {
	if (music == nullptr){
		return false;
	} else {
		return true;
	}
}

void Music::SetVolume(int volume) {
	if(volume >= 0 && volume <= 100)
		this->volume = volume;

	Mix_VolumeMusic((this->volume/100.0) * MIX_MAX_VOLUME);
}

int Music::GetVolume(){
	return volume;
}

Music::~Music() {
	Stop(0);
}