#include "Tapu.h"
#include "InputManager.h"
#include "Game.h"
#include "Bullet.h"
#include "Camera.h"
#include "Sprite.h"
#include "Collider.h"
#include "Timer.h"

Tapu::Tapu(GameObject& associated, std::weak_ptr<GameObject> Yawara) : Component(associated) {

	Sprite* sp = new Sprite(associated, "assets/img/spirit_r.png", 12, 0.07);
	associated.AddComponent(sp);
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);

	angle = 0;
	this->yawara = Yawara;
    radius = 100;
}

void Tapu::Update(float dt) {
	static Timer cd;
	InputManager& input = InputManager::GetInstance();

	std::shared_ptr<GameObject> pb = yawara.lock();
	if (pb) {

		Vec2 newPos(input.GetMouseX() + Camera::pos.x, input.GetMouseY() + Camera::pos.y);
		Vec2 dist = newPos - pb->box.Center();

		angle = 0;
		associated.angleDeg = 0;

		if(dist.Modulo() > radius){
			newPos = pb->box.Center() + ((dist/dist.Modulo()) * radius);
			angle = atan2(input.GetMouseY() + Camera::pos.y - associated.box.Center().y, input.GetMouseX() + Camera::pos.x - associated.box.Center().x);
			associated.angleDeg = angle/0.0174533;
		}
		associated.box.Centered(newPos);
		cd.Update(dt);
		if(input.MousePress(LEFT_MOUSE_BUTTON) && cd.Get() > PCANNON_SHOOT_CD){
			Shoot();
			cd.Restart();
		}

		if(associated.angleDeg >= 360){
			associated.angleDeg -= 360;
		}
		if(associated.angleDeg <= -360){
			associated.angleDeg += 360;
		}
	} else {
		associated.RequestDelete();
	}
}

void Tapu::Render() {

}

bool Tapu::Is(std::string type) {
	return !strcmp(type.c_str(), "Tapu");
}

void Tapu::Shoot() {
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

void Tapu::NotifyCollision(GameObject& other) {
	if (other.GetComponent("Bullet") && static_cast<Bullet*>(other.GetComponent("Bullet"))->targetsPlayer) {
		
	}
}