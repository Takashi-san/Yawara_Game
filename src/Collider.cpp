#include "Collider.h"
#include <string.h>

#ifdef DEBUG
#include "Camera.h"
#include "Game.h"

#include <SDL2/SDL.h>
#endif // DEBUG

// Copie o conteúdo dessa função para dentro da sua e adapte o nome das funções para as suas.
// Funções usadas:
// Rect::GetCenter()				- Retorna um Vec2 no centro do Rect
// Vec2::operator-( const Vec2& )	- Subtrai dois Vec2
// Vec2::Rotate( float rad )		- Rotaciona um Vec2 pelo ângulo em radianos passado
void Collider::Render() {
#ifdef DEBUG
	Vec2 center( box.Center() );
	SDL_Point points[5];

	Vec2 point = (Vec2(box.x, box.y) - center).GetRotated( associated.angleDeg*0.0174533 ) + center - Camera::pos;
	points[0] = {(int)point.x, (int)point.y};
	points[4] = {(int)point.x, (int)point.y};
	
	point = (Vec2(box.x + box.w, box.y) - center).GetRotated( associated.angleDeg*0.0174533 ) + center - Camera::pos;
	points[1] = {(int)point.x, (int)point.y};
	
	point = (Vec2(box.x + box.w, box.y + box.h) - center).GetRotated( associated.angleDeg*0.0174533 ) + center - Camera::pos;
	points[2] = {(int)point.x, (int)point.y};
	
	point = (Vec2(box.x, box.y + box.h) - center).GetRotated( associated.angleDeg*0.0174533 ) + center - Camera::pos;
	points[3] = {(int)point.x, (int)point.y};

	SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}

Collider::Collider(GameObject& associated, Vec2 scale, Vec2 offset) : Component(associated) {
	this->scale = scale;
	this->offset = offset;

	box.w = associated.box.w * scale.x;
	box.h = associated.box.h * scale.y;
}

void Collider::Update(float dt) {
	box.Centered(associated.box.Center());

	box.x += offset.GetRotated(associated.angleDeg/0.0174533).x;
	box.y += offset.GetRotated(associated.angleDeg/0.0174533).y;
}

bool Collider::Is(std::string type) {
	return !strcmp(type.c_str(), "Collider");
}

void Collider::SetOffset(Vec2 offset) {
	this->offset = offset;
}

void Collider::SetScale(Vec2 scale) {
	this->scale = scale;

	box.w = associated.box.w * scale.x;
	box.h = associated.box.h * scale.y;
}