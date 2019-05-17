#include "PenguinCannon.h"
#include "InputManager.h"
#include "Game.h"
#include "Bullet.h"
#include "Camera.h"
#include "Sprite.h"
#include "Collider.h"
#include "Timer.h"

PenguinCannon::PenguinCannon(GameObject& associated, std::weak_ptr<GameObject> penguinBody) : Component(associated) {
	Sprite* sp = new Sprite(associated, "assets/img/cubngun.png");
	associated.AddComponent(sp);
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);

	angle = 0;
	pbody = penguinBody;
}

void PenguinCannon::Update(float dt) {
	static Timer cd;
	InputManager& input = InputManager::GetInstance();

	std::shared_ptr<GameObject> pb = pbody.lock();
	if (pb) {
		associated.box.Centered(pb->box.Center());

		angle = atan2(input.GetMouseY() + Camera::pos.y - associated.box.Center().y, input.GetMouseX() + Camera::pos.x - associated.box.Center().x);
		associated.angleDeg = angle/0.0174533;

		cd.Update(dt);
		if (input.MousePress(LEFT_MOUSE_BUTTON)) {
			if (cd.Get() > PCANNON_SHOOT_CD) {
				Shoot();
				cd.Restart();
			}	
		}
	} else {
		associated.RequestDelete();
	}
}

void PenguinCannon::Render() {

}

bool PenguinCannon::Is(std::string type) {
	return !strcmp(type.c_str(), "PenguinCannon");
}

void PenguinCannon::Shoot() {
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;

	GameObject *go = new GameObject();
	weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
	ptr = weak_ptr.lock();

	Bullet *bam = new Bullet(*ptr, angle, PCANNON_BULLET_SPEED, PCANNON_BULLET_DAMAGE, PCANNON_BULLET_RANGE, "assets/img/minionbullet2.png", 3, 0.04, false);
	ptr->box.Centered(associated.box.Center());
	Vec2 offset(associated.box.w/2, 0);
	offset.Rotate(angle);
	ptr->box.x += offset.x;
	ptr->box.y += offset.y;
	ptr->AddComponent(bam);
}

void PenguinCannon::NotifyCollision(GameObject& other) {
	if (other.GetComponent("Bullet") && static_cast<Bullet*>(other.GetComponent("Bullet"))->targetsPlayer) {
		
	}
}