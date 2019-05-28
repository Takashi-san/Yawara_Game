#include "Yawara.h"

#include "Game.h"
#include "Sprite.h"
#include "Collider.h"
#include "InputManager.h"
#include "Camera.h"
#include "Bullet.h"
#include "Sound.h"
#include "Tapu.h"

Yawara* Yawara::player;

Yawara::Yawara(GameObject& associated) : Component(associated) {
	player = this;

	Sprite* sp = new Sprite(associated, "assets/img/guara_r.png", 12, 0.100);
	associated.AddComponent(sp);
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);

	hp = 100;
	speed = {0, 0};
	dir = RIGHT;
	idle = true;
}

Yawara::~Yawara() {
	player = nullptr;
}

void Yawara::Start() {

	GameObject *go = new GameObject();
	std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
	std::shared_ptr<GameObject> ptr = weak_ptr.lock();
	tapu = weak_ptr;

	Tapu* tp = new Tapu(*ptr, Game::GetInstance().GetCurrentState().GetObjectPtr(&associated));
	ptr->box.x = associated.box.x;
	ptr->box.y = associated.box.y;
	ptr->AddComponent(tp);

}

void Yawara::Update(float dt) {
	InputManager& input = InputManager::GetInstance();

	if (input.IsKeyDown(W_KEY)) {
		if (input.IsKeyDown(A_KEY)) {
			speed = {-YAWARA_SPEED/2, -YAWARA_SPEED/2};
			new_dir = LEFT_UP;
		} else if (input.IsKeyDown(D_KEY)) {
			speed = {YAWARA_SPEED/2, -YAWARA_SPEED/2};
			new_dir = RIGHT_UP;
		} else {
			speed = {0, -YAWARA_SPEED};
			new_dir = UP;
		}
		idle = false;
	} else if (input.IsKeyDown(S_KEY)) {
		if (input.IsKeyDown(A_KEY)) {
			speed = {-YAWARA_SPEED/2, YAWARA_SPEED/2};
			new_dir = LEFT_DOWN;
		} else if (input.IsKeyDown(D_KEY)) {
			speed = {YAWARA_SPEED/2, YAWARA_SPEED/2};
			new_dir = RIGHT_DOWN;
		} else {
			speed = {0, YAWARA_SPEED};
			new_dir = DOWN;
		}
		idle = false;
	} else if (input.IsKeyDown(A_KEY)) {
		speed = {-YAWARA_SPEED, 0};
		new_dir = LEFT;
		idle = false;
	} else if (input.IsKeyDown(D_KEY)) {
		speed = {YAWARA_SPEED, 0};
		new_dir = RIGHT;
		idle = false;
	} else {
		speed = {0, 0};
		new_dir = dir;
		idle = true;
	}

	if (idle) {	
		Sprite* sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));
		if (sp) {
			sp->Open("assets/img/idle.png");
			sp->SetFrameCount(6);
		}
	} else if (new_dir != dir) {
		dir = new_dir;

		Sprite* sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));
		if (sp) {
			switch (dir) {
				case RIGHT:
					sp->Open("assets/img/guara_r.png");
					sp->SetFrameCount(12);
				break;

				case LEFT:
					sp->Open("assets/img/guara_l.png");
					sp->SetFrameCount(12);
				break;

				case UP:
					sp->Open("assets/img/walk_up.png");
					sp->SetFrameCount(5);
				break;

				case DOWN:
					sp->Open("assets/img/walk_down.png");
					sp->SetFrameCount(5);
				break;

				case RIGHT_UP:
				break;

				case RIGHT_DOWN:
				break;

				case LEFT_UP:
				break;

				case LEFT_DOWN:
				break;

				default:
				break;
			}
		}
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