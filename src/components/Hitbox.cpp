#include "Hitbox.h"

Hitbox::Hitbox(GameObject& associated, int damage, bool targetsPlayer, float selfDestruct, bool hitDie) : Component(associated) {
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);
	colisor = cl;

	this->selfDestruct = selfDestruct;
	this->targetsPlayer = targetsPlayer;
	this->damage = damage;
	this->hitDie = hitDie;
}

void Hitbox::Update(float dt) {
	if (selfDestruct != 0) {
		SFTimer.Update(dt);
		if (SFTimer.Get() >= selfDestruct) {
			associated.RequestDelete();
		}
	}
}

void Hitbox::Render() {
}

bool Hitbox::Is(std::string type) {
	return !strcmp(type.c_str(), "Hitbox");
}

int Hitbox::GetDamage() {
	return damage;
}

void Hitbox::SetDamage(int damage) {
	this->damage = damage;
}

void Hitbox::SetSelfDestruct(float selfDestruct) {
	this->selfDestruct = selfDestruct;
}
void Hitbox::NotifyCollision(GameObject &other)
{
	if (hitDie) {
		if (other.GetComponent("Capelobo") && !targetsPlayer)
			associated.RequestDelete();

		if (other.GetComponent("Yawara") && targetsPlayer)
			associated.RequestDelete();
	}
}