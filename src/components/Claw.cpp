#include "Claw.h"
#include "Sprite.h"
#include "Collider.h"
#include "Easing.h"

Claw::Claw(GameObject &associated, float angle, float speed, int damage, weak_ptr<GameObject> center, float radius, float width, float high, bool targetsPlayer) : Component(associated)
{
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);

	this->targetsPlayer = targetsPlayer;
	this->speed = speed;
	this->damage = damage;
	this->radius = radius;
	this->angleLeft = 90;
	this->atAngle = angle;
	this->center = center;

	duration.Restart();
}

void Claw::Update(float dt)
{

	duration.Update(dt);
	if (duration.Get() > 1)
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

	// if (other.GetComponent("Yawara") && targetsPlayer)
	// 	associated.RequestDelete();
}