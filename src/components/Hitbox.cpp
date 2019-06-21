#include "Hitbox.h"

Hitbox::Hitbox(GameObject& associated, bool targetsPlayer, float selfDestruct) : Component(associated) {
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);
	colisor = cl;

	this->selfDestruct = selfDestruct;
	this->targetsPlayer = targetsPlayer;
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

void Hitbox::SetSelfDestruct(float selfDestruct) {
	this->selfDestruct = selfDestruct;
}
