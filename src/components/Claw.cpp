#include "Claw.h"
#include "Sprite.h"
#include "Collider.h"
#include "Easing.h"

Claw::Claw(GameObject &associated, int damage, bool targetsPlayer) : Hitbox(associated, damage, targetsPlayer, 1)
{
	this->targetsPlayer = targetsPlayer;
	this->damage = damage;

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

void Claw::NotifyCollision(GameObject &other)
{
	if (other.GetComponent("Capelobo") && !targetsPlayer)
		associated.RequestDelete();

	if (other.GetComponent("Yawara") && targetsPlayer)
		associated.RequestDelete();
}