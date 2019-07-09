#include "Sprite.h"
#include "Game.h"
#include "Resources.h"
#include "Camera.h"

Sprite::Sprite(GameObject& associated) : Component(associated){
	texture = nullptr;
	scale.x = 1;
	scale.y = 1;
	width = 0;
	height = 0;

	frameCount = 1;
	frameTime = 0;
	currentFrame = 0;
	secondsToSelfDestruct = 0;
	stopFrame = -1;
	stopFlag = false;
	alpha = 255;
	r_mod = 255;
	g_mod = 255;
	b_mod = 255;
	blend = BLEND_BLEND;
}

Sprite::Sprite(GameObject& associated, std::string file, int frameCount, float frameTime, float secondsToSelfDestruct, int stopFrame) : Component(associated){
	texture = nullptr;
	scale.x = 1;
	scale.y = 1;
	width = 0;
	height = 0;
	
	if (frameCount <= 0) {
		this->frameCount = 1;
	} else {
		this->frameCount = frameCount;
	}

	if (frameTime < 0) {
		this->frameTime = 0;
	} else {
		this->frameTime = frameTime;
	}

	if (stopFrame < this->frameCount) {
		this->stopFrame = stopFrame;
	} else {
		this->stopFrame = -1;
	}

	currentFrame = 0;
	if (this->stopFrame == currentFrame) {
		stopFlag = true;
	} else {
		stopFlag = false;
	}

	alpha = 255;
	r_mod = 255;
	g_mod = 255;
	b_mod = 255;
	blend = BLEND_BLEND;
	this->secondsToSelfDestruct = secondsToSelfDestruct;

	Open(file);
}

Sprite::~Sprite() {
	// corrige alpha, color mod e blend da textura que não vai mais utilizar.
	if (texture != nullptr) {
		SDL_SetTextureAlphaMod(texture.get(), 255);
		SDL_SetTextureColorMod(texture.get(), 255, 255, 255);
		SDL_SetTextureBlendMode(texture.get(), BLEND_BLEND);
	}
}

void Sprite::Open(std::string file) {
	// corrige alpha, color mod e blend da textura que não vai mais utilizar.
	if (texture != nullptr) {
		SDL_SetTextureAlphaMod(texture.get(), 255);
		SDL_SetTextureColorMod(texture.get(), 255, 255, 255);
		SDL_SetTextureBlendMode(texture.get(), BLEND_BLEND);
	}

	// carrega textura
	texture = Resources::GetImage(file.c_str());
	if (texture == nullptr) {
		// falha em carregar imagem.
		std::cout << "Falha em carregar textura: " << file.c_str() << "\n";
		std::cout << "SDL_GetError: " << SDL_GetError() << "\n";
		//exit(EXIT_FAILURE);
	}

	// aplica alpha, color mod e blend.
	SDL_SetTextureAlphaMod(texture.get(), alpha);
	SDL_SetTextureColorMod(texture.get(), r_mod, g_mod, b_mod);
	SDL_SetTextureBlendMode(texture.get(), blend);

	// descobre dimensoes da imagem.
	SDL_QueryTexture(texture.get(), nullptr, nullptr, &width, &height);

	// clip da imagem.
	SetClip(0, 0, width/frameCount, height);

	// seta altura e largura no gameobject.
	associated.box.w = (width/frameCount)*scale.y;
	associated.box.h = (height)*scale.x;
}

void Sprite::SetClip(int x, int y, int w, int h) {
	clipRect.x = x;
	clipRect.y = y;
	clipRect.w = w;
	clipRect.h = h;
}

void Sprite::Render() {
	Render(associated.box.x - Camera::pos.x, associated.box.y - Camera::pos.y);
}

void Sprite::Render(int x, int y) {
	Game& instance = Game::GetInstance();
	float tmp;

	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = clipRect.w*scale.x;
	dst.h = clipRect.h*scale.y;

	// Renderiza apenas o que pode ser visível na tela.
	tmp = Vec2(dst.w, dst.h).Modulo();
	if ((x < -tmp) || (y < -tmp)) {
		return;
	} else if ((x > (tmp + instance.GetWindowSize().x)) || (y > (tmp + instance.GetWindowSize().y))) {
		return;
	}

	SDL_RenderCopyEx(instance.GetRenderer(), texture.get(), &clipRect, &dst, associated.angleDeg, nullptr, SDL_FLIP_NONE);
}

int Sprite::GetWidth() {
	return (width/frameCount)*scale.x;
}

int Sprite::GetHeight() {
	return height*scale.y;
}

bool Sprite::IsOpen() {
	if (texture != nullptr) {
		return true;
	} else {
		return false;
	}
}

void Sprite::Update(float dt){
	if ((frameTime != 0) && (!stopFlag)) {
		timeCount.Update(dt);
		if (timeCount.Get() >= frameTime) {
			timeCount.Restart();
			currentFrame++;
			if (currentFrame == frameCount) {
				currentFrame = 0;
			}
			if (currentFrame == stopFrame) {
				stopFlag = true;
			}
			SetClip((width/frameCount)*currentFrame, 0, width/frameCount, height);
		}
	}

	if (secondsToSelfDestruct != 0) {
		selfDestructCount.Update(dt);
		if (selfDestructCount.Get() > secondsToSelfDestruct) {
			associated.RequestDelete();
		}
	}
}

bool Sprite::Is(std::string type) {
	return !strcmp(type.c_str(), "Sprite");
}

void Sprite::SetScale(float scaleX, float scaleY) {
	if (scaleX != 0) {
		scale.x = scaleX;
		associated.box.w = GetWidth();
	}
	if (scaleY != 0) {
		scale.y = scaleY;
		associated.box.h = GetHeight();
	}
}

void Sprite::SetScale(Vec2 arg) {
	SetScale(arg.x, arg.y);
}

Vec2 Sprite::GetScale() {
	return scale;
}

void Sprite::SetFrame(int frame) {
	if (frame < frameCount && frame >= 0) {
		timeCount.Restart();
		currentFrame = frame;
		SetClip((width/frameCount)*currentFrame, 0, width/frameCount, height);
		
		if (currentFrame == stopFrame) {
			stopFlag = true;
		}
	}
}

void Sprite::SetFrameCount(int frameCount) {
	if (frameCount > 0) {
		this->frameCount = frameCount;
		timeCount.Restart();
		currentFrame = 0;
		stopFrame = -1;
		stopFlag = false;
		SetClip((width/frameCount)*currentFrame, 0, width/frameCount, height);
		associated.box.w = width/frameCount;
	}
}

void Sprite::SetFrameTime(float frameTime) {
	if (frameTime >= 0) {
		this->frameTime = frameTime;
		timeCount.Restart();
	}
}

void Sprite::SetStopFrame(int stopFrame) {
	if (stopFrame < frameCount) {
		this->stopFrame = stopFrame;
		timeCount.Restart();

		if (currentFrame == stopFrame) {
			stopFlag = true;
		}
	}
}

void Sprite::Stop() {
	stopFlag = true;
}

void Sprite::Resume() {
	stopFlag = false;
}

bool Sprite::IsStop() {
	return stopFlag;
}

void Sprite::SetAlphaMod(int alpha) {
	if ((alpha >= 0) && (alpha <= 255)) {
		this->alpha = (Uint8)alpha;
		SDL_SetTextureAlphaMod(texture.get(), this->alpha);
	}
}

int Sprite::GetAlphaMod() {
	Uint8 alpha;
	int valid;
	valid = SDL_GetTextureAlphaMod(texture.get(), &alpha);
	if (valid == 0) {
		return alpha;
	} else {
		return -1;
	}
}

void Sprite::SetColorMod(int r, int g, int b) {
	if ((r >= 0) && (r <= 255) && (g >= 0) && (g <= 255) && (b >= 0) && (b <= 255)) {
		r_mod = (Uint8)r;
		g_mod = (Uint8)g;
		b_mod = (Uint8)b;
		SDL_SetTextureColorMod(texture.get(), r_mod, g_mod, b_mod);
	}
}

void Sprite::GetColorMod(int* r, int* g, int* b) {
	int valid;
	Uint8 rm, gm, bm;
	valid = SDL_GetTextureColorMod(texture.get(), &rm, &gm, &bm);
	if (valid == 0) {
		*r = rm;
		*g = gm;
		*b = bm;
	} else {
		*r = -1;
		*g = -1;
		*b = -1;
	}
}

void Sprite::SetBlendMode(int mode) {
	if ((mode >= BLEND_NONE) && (mode <= BLEND_MOD) && (mode != 3)) {
		blend = (SDL_BlendMode)mode;
		SDL_SetTextureBlendMode(texture.get(), blend);
	}
}

void Sprite::SetSelfDestruct(float secondsToSelfDestruct) {
	this->secondsToSelfDestruct = secondsToSelfDestruct;
}