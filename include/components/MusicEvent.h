#pragma once

#include "Component.h"

#define INCLUDE_SDL
#include "SDL_include.h"
#include "Music.h"
#include "State.h"
#include "TileSet.h"
#include "Sprite.h"

class MusicEvent : public Component
{
private:
    Music bgMusic;
    std::string musica;
public:
    MusicEvent(GameObject&, float, float, float, float, std::string);

    void Update(float);
	void Render();
	bool Is(std::string);
	void NotifyCollision(GameObject &);

	void Start();
};
