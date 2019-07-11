#include "Sound.h"
#include "Resources.h"

Sound::Sound(GameObject& associated) : Component(associated){
	chunk = nullptr;
}

Sound::Sound(GameObject& associated, std::string file) : Sound(associated){
	Open(file);
}

void Sound::Play(int times, int volume) {
	if (chunk != nullptr){
		// Mix_PlayChannel(canal, Mix_Chunk*, loops)
		if (times > 0) {
			channel = Mix_PlayChannel(-1, chunk.get(), times-1);
			// volume de 0 a 128;
			Mix_Volume(channel, volume);
		}
	} else {
		// Nenhum som carregado.
		std::cout << "Nenhum som carregado para tocar.\n";
	}
}

void Sound::Stop() {
	if (chunk != nullptr) {
		Mix_HaltChannel(channel);
	}
}

void Sound::Open(std::string file) {
	chunk = Resources::GetSound(file.c_str());
	if (chunk == nullptr) {
		// Falha em carregar o som.
		std::cout << "Falha em carregar sound: " << file.c_str() << "\n";
		std::cout << "SDL_GetError: " << SDL_GetError() << "\n";
	}
}

void Sound::FadeOut(int ms) {
	Mix_FadeOutChannel(channel, ms);
}

void Sound::PlayFadeIn(int ms, int volume, int times, int ticks) {
	if (chunk != nullptr){
		if (times > 0) {
			channel = Mix_FadeInChannelTimed(-1, chunk.get(), times-1, ms, ticks);
			// volume de 0 a 128;
			//Mix_Volume(channel, volume);
		}
	} else {
		// Nenhum som carregado.
		std::cout << "Nenhum som carregado para tocar.\n";
	}
}

bool Sound::IsOpen() {
	if (chunk == nullptr){
		return false;
	} else {
		return true;
	}
}

Sound::~Sound() {
	if (chunk != nullptr) {
		Mix_HaltChannel(channel);
		//Mix_FreeChunk(chunk);
	}
}

void Sound::Update(float dt) {
	
}

void Sound::Render() {

}

bool Sound::Is(std::string type) {
	return !strcmp(type.c_str(), "Sound");
}