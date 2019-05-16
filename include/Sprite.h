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

#ifndef SPRITE
	#define SPRITE

	class Sprite : public Component{
	private:
		std::shared_ptr<SDL_Texture> texture;
		int width;
		int height;
		SDL_Rect clipRect;
		Vec2 scale;

		int frameCount;
		int currentFrame;
		float timeElapsed;
		float frameTime;

		Timer selfDestructCount;
		float secondsToSelfDestruct;

	public:
		Sprite(GameObject&);
		Sprite(GameObject&, std::string, int = 1, float = 1, float = 0);
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

		void Update(float dt);
		bool Is(std::string type);
	};
	
#endif