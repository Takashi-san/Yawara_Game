#include "Bullet.h"
#include "Sprite.h"
#include "Collider.h"

Bullet::Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance, std::string sprite, std::string spriteEnd, int frameCount, float frameTime, bool targetsPlayer) : Component(associated) {
	Sprite* sp = new Sprite(associated, sprite, frameCount, frameTime);
	sp->SetScale(1, 1);
	Collider *cl = new Collider(associated);
	associated.AddComponent(sp);
	associated.AddComponent(cl);

	this->targetsPlayer = targetsPlayer; 
	this->speed.x = speed;
	this->speed.y = 0;
	this->speed.Rotate(angle);
	associated.angleDeg = angle * (180/PI);
	this->damage = damage;
	this->distanceLeft = maxDistance;
	this->spriteEnd = spriteEnd;
}

void Bullet::Update(float dt) {
	associated.box.x += speed.x*dt;
	associated.box.y += speed.y*dt;
	distanceLeft -= speed.Modulo()*dt;
	if (distanceLeft <= 0) {
		associated.RequestDelete();
	}
}

void Bullet::Render() {

}

bool Bullet::Is(std::string type) {
	return !strcmp(type.c_str(), "Bullet");
}

int Bullet::GetDamage() {
	return damage;
}

void Bullet::NotifyCollision(GameObject& other) {
	if ((other.GetComponent("Enemy") && !targetsPlayer) || (other.GetComponent("Yawara") && targetsPlayer)) {
		Vec2 pos = associated.box.Center();
		Sprite *sp = static_cast<Sprite *>(associated.GetComponent("Sprite"));
		sp->Open(spriteEnd);
		sp->SetFrameCount(6);
		sp->SetFrameTime(0.05);
		sp->SetSelfDestruct(6*0.05);
		speed.x = speed.x*0.3;
		speed.y = speed.y*0.3;
		associated.box.Centered(pos);

		Collider *cl = static_cast<Collider *>(associated.GetComponent("Collider"));
		associated.RemoveComponent(cl);
	}
}