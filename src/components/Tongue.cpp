#include "Tongue.h"
#include "Sprite.h"
#include "Collider.h"
#include "Easing.h"

bool hitMax;

Tongue::Tongue(GameObject &associated, int damage, float speed, int angleDeg, float maxDistance, bool targetsPlayer) : Component(associated)
{
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);

	this->damage = damage;
	angle = angleDeg;
	distanceLeft = maxDistance;
	this->maxDistance = maxDistance;
	this->targetsPlayer = targetsPlayer;

	associated.angleDeg = angle + 90;
	
	float rad_angle = angle * (PI / 180);
    this->speed = Vec2(speed * cos(rad_angle), speed * sin(rad_angle));
	hitMax = false;
}

void Tongue::Update(float dt)
{
	if(distanceLeft > 0 && !hitMax){
		associated.box += speed * dt;
    	distanceLeft -= (speed * dt).Modulo();
	}
	else if(!hitMax){
		hitMax = true;
		distanceLeft = maxDistance;
	}
	else if(distanceLeft > 0 && hitMax){
		associated.box -= speed * dt;
    	distanceLeft -= (speed * dt).Modulo();
	}
	
	if (hitMax && distanceLeft < 0)
	{
		associated.RequestDelete();
	}
}

void Tongue::Render()
{
}

bool Tongue::Is(std::string type)
{
	return !strcmp(type.c_str(), "Tongue");
}

int Tongue::GetDamage()
{
	return damage;
}

void Tongue::NotifyCollision(GameObject &other)
{
	if (other.GetComponent("Capelobo") && !targetsPlayer)
		associated.RequestDelete();

	// if (other.GetComponent("Yawara") && targetsPlayer)
	// 	associated.RequestDelete();
}