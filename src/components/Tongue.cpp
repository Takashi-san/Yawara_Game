#include "Tongue.h"
#include "Sprite.h"
#include "Collider.h"
#include "Easing.h"
#include "Sound.h"
#include "Game.h"

bool hitMax;
bool played;

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
	played = false;
}

void Tongue::Update(float dt)
{
	if(distanceLeft > 0 && !hitMax){
		associated.box += speed * dt;
    	distanceLeft -= (speed * dt).Modulo();
	}
	else if(!hitMax){
		GameObject *soundGO = new GameObject();
		std::weak_ptr<GameObject> weak_claw = Game::GetInstance().GetCurrentState().AddObject(soundGO);
		std::shared_ptr<GameObject> shared_claw = weak_claw.lock();
		Sound *so = new Sound(*shared_claw, "assets/audio/sons/capelobo/LINGUA BATE.ogg");
		shared_claw->AddComponent(so);
		so->Play(1,MIX_MAX_VOLUME);
		played = true;
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
	if (other.GetComponent("Yawara") && targetsPlayer){
		GameObject *soundGO = new GameObject();
		std::weak_ptr<GameObject> weak_claw = Game::GetInstance().GetCurrentState().AddObject(soundGO);
		std::shared_ptr<GameObject> shared_claw = weak_claw.lock();
		Sound *so = new Sound(*shared_claw, "assets/audio/sons/capelobo/LINGUA BATE.ogg");
		shared_claw->AddComponent(so);
		if(!played)so->Play(1,MIX_MAX_VOLUME);
		associated.RequestDelete();
	}
}