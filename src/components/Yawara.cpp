#include "Yawara.h"

#include "Game.h"
#include "Sprite.h"
#include "Collider.h"
#include "InputManager.h"
#include "Camera.h"
#include "Bullet.h"
#include "Sound.h"

Yawara* Yawara::player;

Yawara::Yawara(GameObject& associated) : Component(associated) {
	player = this;

	Sprite* sp = new Sprite(associated, "assets/img/guara_r.png", 12, 0.070);
	associated.AddComponent(sp);
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);

	hp = 100;
	speed = {0, 0};
	dir = RIGHT;
}

Yawara::~Yawara() {
	player = nullptr;
}

void Yawara::Start() {

}

void Yawara::Update(float dt) {
	InputManager& input = InputManager::GetInstance();

	if (input.IsKeyDown(W_KEY)) {
		speed = {0, -YAWARA_SPEED};
		
		if (input.IsKeyDown(A_KEY)) {
			speed = {-YAWARA_SPEED/2, -YAWARA_SPEED/2};
		} else if (input.IsKeyDown(D_KEY)) {
			speed = {YAWARA_SPEED/2, -YAWARA_SPEED/2};
		}
	} else if (input.IsKeyDown(S_KEY)) {
		speed = {0, YAWARA_SPEED};

		if (input.IsKeyDown(A_KEY)) {
			speed = {-YAWARA_SPEED/2, YAWARA_SPEED/2};
		} else if (input.IsKeyDown(D_KEY)) {
			speed = {YAWARA_SPEED/2, YAWARA_SPEED/2};
		}
	} else if (input.IsKeyDown(A_KEY)) {
		speed = {-YAWARA_SPEED, 0};
	} else if (input.IsKeyDown(D_KEY)) {
		speed = {YAWARA_SPEED, 0};
	} else {
		speed = {0, 0};
	}

	associated.box.x += speed.x*dt;
	associated.box.y += speed.y*dt;

	if (hp <= 0) {
		associated.RequestDelete();
		Camera::Unfollow();

		GameObject *go = new GameObject();
		std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
		std::shared_ptr<GameObject> ptr = weak_ptr.lock();
		
		Sprite* sp = new Sprite(*ptr, "assets/img/penguindeath.png", 5, 0.1, 5*0.1);
		Sound *so = new Sound(*ptr, "assets/audio/boom.wav");
		ptr->box.Centered(associated.box.Center());
		ptr->AddComponent(sp);
		ptr->AddComponent(so);

		so->Play(1);
	}
}

void Yawara::Render() {

}

bool Yawara::Is(std::string type) {
	return !strcmp(type.c_str(), "Yawara");
}

void Yawara::NotifyCollision(GameObject& other) {
	Bullet* bullet = static_cast<Bullet*>(other.GetComponent("Bullet"));
	
	if (bullet && bullet->targetsPlayer) {
		hp -= bullet->GetDamage();
	}
}

Vec2 Yawara::GetPos() {
	return associated.box.Center();
}