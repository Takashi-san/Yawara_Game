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

	Sprite* sp = new Sprite(associated, "assets/img/yawara_r.png", 12, 0.100);
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

}

void Yawara::Update(float dt) {
	InputManager& input = InputManager::GetInstance();

	if (input.IsKeyDown(W_KEY)) {
		if (input.IsKeyDown(A_KEY)) {
			speed = {-YAWARA_SPEED/2, -YAWARA_SPEED/2};
			if (dir != LEFT_UP) {
				change_sprite = true;
			}
			dir = LEFT_UP;
		} else if (input.IsKeyDown(D_KEY)) {
			speed = {YAWARA_SPEED/2, -YAWARA_SPEED/2};
			if (dir != RIGHT_UP) {
				change_sprite = true;
			}
			dir = RIGHT_UP;
		} else {
			speed = {0, -YAWARA_SPEED};
			if (dir != UP) {
				change_sprite = true;
			}
			dir = UP;
		}

		if (idle) {
			change_sprite = true;
		}
		idle = false;
	} else if (input.IsKeyDown(S_KEY)) {
		if (input.IsKeyDown(A_KEY)) {
			speed = {-YAWARA_SPEED/2, YAWARA_SPEED/2};
			if (dir != LEFT_DOWN) {
				change_sprite = true;
			}
			dir = LEFT_DOWN;
		} else if (input.IsKeyDown(D_KEY)) {
			speed = {YAWARA_SPEED/2, YAWARA_SPEED/2};
			if (dir != RIGHT_DOWN) {
				change_sprite = true;
			}
			dir = RIGHT_DOWN;
		} else {
			speed = {0, YAWARA_SPEED};
			if (dir != DOWN) {
				change_sprite = true;
			}
			dir = DOWN;
		}

		if (idle) {
			change_sprite = true;
		}
		idle = false;
	} else if (input.IsKeyDown(A_KEY)) {
		speed = {-YAWARA_SPEED, 0};
		if (dir != LEFT) {
			change_sprite = true;
		}
		dir = LEFT;

		if (idle) {
			change_sprite = true;
		}
		idle = false;
	} else if (input.IsKeyDown(D_KEY)) {
		speed = {YAWARA_SPEED, 0};
		if (dir != RIGHT) {
			change_sprite = true;
		}
		dir = RIGHT;

		if (idle) {
			change_sprite = true;
		}
		idle = false;
	} else {
		speed = {0, 0};
		
		if (!idle) {
			change_sprite = true;
		}
		idle = true;
	}

	if (change_sprite) {
		change_sprite = false;
		if (idle) {	
			Sprite* sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));
			if (sp) {
				switch (dir) {
					case RIGHT:
						sp->Open("assets/img/yawara_r.png");
						sp->SetFrameCount(12);
					break;

					case LEFT:
						sp->Open("assets/img/yawara_run_l.png");
						sp->SetFrameCount(10);
					break;

					case UP:
						sp->Open("assets/img/yawara_u.png");
						sp->SetFrameCount(1);
					break;

					case DOWN:
						sp->Open("assets/img/yawara_d.png");
						sp->SetFrameCount(1);
					break;

					case RIGHT_UP:
						sp->Open("assets/img/yawara_ur.png");
						sp->SetFrameCount(1);
					break;

					case RIGHT_DOWN:
						sp->Open("assets/img/yawara_dr.png");
						sp->SetFrameCount(1);
					break;

					case LEFT_UP:
						sp->Open("assets/img/yawara_ul.png");
						sp->SetFrameCount(1);
					break;

					case LEFT_DOWN:
						sp->Open("assets/img/yawara_dl.png");
						sp->SetFrameCount(1);
					break;

					default:
					break;
				}
			}
		} else {
			Sprite* sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));
			if (sp) {
				switch (dir) {
					case RIGHT:
						sp->Open("assets/img/yawara_r.png");
						sp->SetFrameCount(12);
					break;

					case LEFT:
						sp->Open("assets/img/yawara_l.png");
						sp->SetFrameCount(12);
					break;

					case UP:
						sp->Open("assets/img/yawara_u.png");
						sp->SetFrameCount(1);
					break;

					case DOWN:
						sp->Open("assets/img/yawara_d.png");
						sp->SetFrameCount(1);
					break;

					case RIGHT_UP:
						sp->Open("assets/img/yawara_ur.png");
						sp->SetFrameCount(1);
					break;

					case RIGHT_DOWN:
						sp->Open("assets/img/yawara_dr.png");
						sp->SetFrameCount(1);
					break;

					case LEFT_UP:
						sp->Open("assets/img/yawara_ul.png");
						sp->SetFrameCount(1);
					break;

					case LEFT_DOWN:
						sp->Open("assets/img/yawara_dl.png");
						sp->SetFrameCount(1);
					break;

					default:
					break;
				}
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