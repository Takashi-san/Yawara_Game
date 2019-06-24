#include "Hitbox.h"

Hitbox::Hitbox(GameObject& associated, int damage, bool targetsPlayer, float selfDestruct) : Component(associated) {
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);
	colisor = cl;

	this->damage = damage;
	this->targetsPlayer = targetsPlayer;
}

void Hitbox::Update(float dt) {
}

void Hitbox::Render() {
}

bool Hitbox::Is(std::string type) {
	return !strcmp(type.c_str(), "Hitbox");
}

int Hitbox::GetDamage() {
	return damage;
}

void Hitbox::NotifyCollision(GameObject &other)
{
	if (other.GetComponent("Capelobo") && !targetsPlayer)
		associated.RequestDelete();

	if (other.GetComponent("Yawara") && targetsPlayer)
		associated.RequestDelete();
}