#include "Tongue.h"
#include "Sprite.h"
#include "Collider.h"
#include "Easing.h"

bool hitMax;

Tongue::Tongue(GameObject &associated, int damage, float speed, int angleDeg, float maxDistance, bool targetsPlayer) : Hitbox(associated, damage, true, 0)
{
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

void Tongue::NotifyCollision(GameObject &other)
{
	if (other.GetComponent("Yawara") && targetsPlayer)
		associated.RequestDelete();
}