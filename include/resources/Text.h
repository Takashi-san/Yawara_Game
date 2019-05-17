#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_TTF
#include "SDL_include.h"
#include "GameObject.h"
#include "Timer.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

#ifndef TEXT
	#define TEXT
	
	class Text : public Component{
	public:
		enum TextStyle { SOLID, SHADED, BLENDED };

		Text(GameObject&, std::string, int, TextStyle, std::string, SDL_Color, float = 0, SDL_Color = {0, 0, 0, 255});
		~Text();

		void Update(float);
		void Render();
		bool Is(std::string);

		void SetText(std::string);
		void SetColor(SDL_Color);
		void SetBgColor(SDL_Color);
		void SetStyle(TextStyle);
		void SetFontFile(std::string);
		void SetFontSize(int);

	private:
		std::shared_ptr<TTF_Font> font;
		SDL_Texture* texture;

		std::string text;
		TextStyle style;
		std::string fontFile;
		int fontSize;
		SDL_Color color;
		SDL_Color bgColor;
		Timer time;
		float period;
		uint count;

		void RemakeTexture();
	};
#endif