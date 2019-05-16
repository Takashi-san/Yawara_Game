#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_TTF
#include "SDL_include.h"
#include "State.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <stack>

#ifndef GAME
	#define GAME
		
	class Game {
	private:
		Uint32 frameStart;
		float dt;

		static Game* instance;
		
		State* storedState;
		SDL_Window* window;
		SDL_Renderer* renderer;
		std::stack<std::unique_ptr<State>> stateStack;

		void CalculateDeltaTime();

	public:
		Game(std::string, int, int);
		~Game();

		static Game& GetInstance();
		SDL_Renderer* GetRenderer();
		State& GetCurrentState();

		void Push(State*);

		void Run();
		
		float GetDeltaTime();
	};
	
#endif