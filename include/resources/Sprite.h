#pragma once // Alows to initializate the header just once

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#include "SDL_include.h"
#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"
#include "Timer.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class Sprite : public Component
{
private:
	std::shared_ptr<SDL_Texture> texture;
	int width;
	int height;
	SDL_Rect clipRect;
	Vec2 scale;

	int frameCount;
	int currentFrame;
	
	Timer timeCount;
	float frameTime;
	
	int stopFrame;
	bool stopFlag;

	Uint8 alpha;

	Uint8 r_mod;
	Uint8 g_mod;
	Uint8 b_mod;

	Timer selfDestructCount;
	float secondsToSelfDestruct;

public:
	Sprite(GameObject &);
	Sprite(GameObject &, std::string, int = 1, float = 0, float = 0, int = -1);
	~Sprite();
	void Open(std::string);
	void SetClip(int, int, int, int);
	void Render();
	void Render(int, int);
	int GetWidth();
	int GetHeight();
	bool IsOpen();

	void SetScale(float, float);
	void SetScale(Vec2);
	Vec2 GetScale();

	void SetFrame(int);
	void SetFrameCount(int);
	void SetFrameTime(float);
	void SetStopFrame(int);

	void Stop();
	void Resume();
	bool IsStop();

	void SetAlphaMod(int);
	int GetAlphaMod();

	void SetColorMod(int, int, int);
	void GetColorMod(int*, int*, int*);

	void Update(float dt);
	bool Is(std::string type);
};