#include "Bullet.h"
#include "Sprite.h"
#include "Collider.h"

Bullet::Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance, std::string sprite, int frameCount, float frameTime, bool targetsPlayer) : Component(associated) {
	Sprite* sp = new Sprite(associated, sprite, frameCount, frameTime);
	sp->SetScale(2, 2);
	Collider *cl = new Collider(associated);
	associated.AddComponent(sp);
	associated.AddComponent(cl);

	this->targetsPlayer = targetsPlayer; 
	this->speed.x = speed;
	this->speed.y = 0;
	this->speed.Rotate(angle);
	associated.angleDeg = angle/0.0174533;
	this->damage = damage;
	this->distanceLeft = maxDistance;
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
	if ((other.GetComponent("Minion") || other.GetComponent("Alien")) && !targetsPlayer) {
		associated.RequestDelete();
	}
	if (other.GetComponent("PenguinBody") && targetsPlayer) {
		associated.RequestDelete();
	}
}