#pragma once // Alows to initializate the header just once

#define INCLUDE_SDL
#include "SDL_include.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <unordered_map>

#define LEFT_ARROW_KEY SDLK_LEFT
#define RIGHT_ARROW_KEY SDLK_RIGHT
#define UP_ARROW_KEY SDLK_UP
#define DOWN_ARROW_KEY SDLK_DOWN

#define W_KEY SDLK_w
#define A_KEY SDLK_a
#define S_KEY SDLK_s
#define D_KEY SDLK_d

#define E_KEY SDLK_e
#define Q_KEY SDLK_q

#define SPACE_KEY	SDLK_SPACE
#define ESCAPE_KEY	SDLK_ESCAPE
#define ENTER_KEY	SDLK_RETURN
#define ENTER_KEY2	SDLK_KP_ENTER

#define LEFT_MOUSE_BUTTON SDL_BUTTON_LEFT
#define RIGHT_MOUSE_BUTTON SDL_BUTTON_RIGHT

class InputManager
{
private:
	bool mouseState[6];
	int mouseUpdate[6];

	std::unordered_map<int, bool> keyState;
	std::unordered_map<int, int> keyUpdate;

	bool quitRequested;
	int updateCounter;
	int mouseX;
	int mouseY;
	float mouseS;

	InputManager();
	~InputManager();

public:
	void Update();

	bool KeyPress(int);
	bool KeyRelease(int);
	bool IsKeyDown(int);

	bool MousePress(int);
	bool MouseRelease(int);
	bool IsMouseDown(int);

	int GetMouseX();
	int GetMouseY();
	void SetMouseS(float);

	bool QuitRequested();

	static InputManager &GetInstance();
};