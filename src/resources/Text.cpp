#include "Text.h"
#include "Resources.h"
#include "Camera.h"
#include "Game.h"

Text::Text(GameObject& associated, std::string fontFile, int fontSize, TextStyle style, std::string text, SDL_Color color, float period, SDL_Color bgColor) : Component(associated) {
	this->text = text;
	this->style = style;
	this->fontFile = fontFile;
	this->fontSize = fontSize;
	this->color = color;
	this->bgColor = bgColor;
	this->period = period;

	font = nullptr;
	texture = nullptr;
	count = 0;

	RemakeTexture();
}

Text::~Text() {
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
	}
}

void Text::Update(float dt) {
	// Blink
	if (period != 0) {
		time.Update(dt);
		if (time.Get() > period) {
			time.Restart();
			count++;
			SDL_SetTextureAlphaMod(texture, 255*(count%2));
		}
	}
}

void Text::Render() {
	if (texture != nullptr) {
		Game& instance = Game::GetInstance();
		SDL_Rect clipRect = {0, 0, (int)associated.box.w, (int)associated.box.h};

		SDL_Rect dst;
		dst.x = associated.box.x - Camera::pos.x;
		dst.y = associated.box.y - Camera::pos.y;
		dst.w = clipRect.w;
		dst.h = clipRect.h;
		
		SDL_RenderCopyEx(instance.GetRenderer(), texture, &clipRect, &dst, associated.angleDeg, nullptr, SDL_FLIP_NONE);
	}
}

void Text::SetText(std::string text) {
	this->text = text;
	RemakeTexture();
}

void Text::SetColor(SDL_Color color) {
	this->color = color;
	RemakeTexture();
}

void Text::SetBgColor(SDL_Color bgColor) {
	this->bgColor = bgColor;
	RemakeTexture();
}

void Text::SetStyle(TextStyle style) {
	this->style = style;
	RemakeTexture();
}

void Text::SetFontSize(int fontSize) {
	this->fontSize = fontSize;
	RemakeTexture();
}

void Text::SetFontFile(std::string fontFile) {
	this->fontFile = fontFile;
	RemakeTexture();
}

void Text::RemakeTexture() {
	// Destroi textura existente.
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

	// Tenta obter a fonte.
	font = Resources::GetFont(fontFile, fontSize);
	if (font == nullptr) {
		// falha em carregar fonte.
		std::cout << "Falha em carregar fonte: " << fontFile.c_str() << "\n";
		std::cout << "SDL_GetError: " << SDL_GetError() << "\n";
		return;
	}

	// Cria textura nova.
	SDL_Surface* tmp;
	switch (style) {
		case SOLID:
			tmp = TTF_RenderText_Solid(font.get(), text.c_str(), color);
		break;

		case SHADED:
			tmp = TTF_RenderText_Shaded(font.get(), text.c_str(), color, bgColor);
		break;

		case BLENDED:
			tmp = TTF_RenderText_Blended(font.get(), text.c_str(), color);
		break;
	}
	texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), tmp);
	associated.box.w = tmp->w;
	associated.box.h = tmp->h;
	SDL_FreeSurface(tmp);

}

bool Text::Is(std::string type) {
	return !strcmp(type.c_str(), "Text");
}