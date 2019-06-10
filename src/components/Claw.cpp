#include "Claw.h"
#include "Sprite.h"
#include "Collider.h"
#include "Easing.h"

Claw::Claw(GameObject &associated, float angle, float speed, int damage, Vec2 center, float radius, float width, float high, bool targetsPlayer) : Component(associated)
{
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);

	this->targetsPlayer = targetsPlayer;
	this->speed.x = speed;
	this->speed.y = 0;
	this->speed.Rotate(angle);
	associated.angleDeg = angle / (PI / 180);
	this->damage = damage;
	this->distanceLeft = PI * radius / 2;
	this->center = center;
}

void Claw::Update(float dt)
{

	associated.box.x += CircularEaseIn(speed.x * dt);
	associated.box.y += CircularEaseIn(speed.y * dt);
	distanceLeft -= speed.Modulo() * dt;
	if (distanceLeft <= 0)
	{
		associated.RequestDelete();
	}
}

void Claw::Render()
{
}

bool Claw::Is(std::string type)
{
	return !strcmp(type.c_str(), "Claw");
}

int Claw::GetDamage()
{
	return damage;
}

void Claw::NotifyCollision(GameObject &other)
{
	if (other.GetComponent("Capelobo") && !targetsPlayer)
		associated.RequestDelete();

	if (other.GetComponent("PenguinBody") && targetsPlayer)
		associated.RequestDelete();
}