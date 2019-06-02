#include "Yawara.h"

#include "Game.h"
#include "Sprite.h"
#include "Collider.h"
#include "InputManager.h"
#include "Camera.h"
#include "Bullet.h"
#include "Sound.h"
#include "Tapu.h"

#define YWR_SPEED		500
#define YWR_HP			100

#define YWR_ANI_TIME	0.100

#define YWR_IDLE_D			"assets/img/yawara/yawara_idle_down.png"
#define YWR_IDLE_D_FRAME	1
#define YWR_IDLE_U			"assets/img/yawara/yawara_idle_up.png"
#define YWR_IDLE_U_FRAME	1
#define YWR_IDLE_L			"assets/img/yawara/yawara_walk_left.png"
#define YWR_IDLE_L_FRAME	12
#define YWR_IDLE_R			"assets/img/yawara/yawara_walk_right.png"
#define YWR_IDLE_R_FRAME	12
#define YWR_IDLE_DL			"assets/img/yawara/yawara_idle_down_left.png"
#define YWR_IDLE_DL_FRAME	1
#define YWR_IDLE_DR			"assets/img/yawara/yawara_idle_down_right.png"
#define YWR_IDLE_DR_FRAME	1
#define YWR_IDLE_UL			"assets/img/yawara/yawara_idle_up_left.png"
#define YWR_IDLE_UL_FRAME	1
#define YWR_IDLE_UR			"assets/img/yawara/yawara_idle_up_right.png"
#define YWR_IDLE_UR_FRAME	1

#define YWR_RUN_D			"assets/img/yawara/yawara_run_down.png"
#define YWR_RUN_D_FRAME		9
#define YWR_RUN_U			"assets/img/yawara/yawara_run_up.png"
#define YWR_RUN_U_FRAME		9
#define YWR_RUN_L			"assets/img/yawara/yawara_run_left.png"
#define YWR_RUN_L_FRAME		10
#define YWR_RUN_R			"assets/img/yawara/yawara_walk_right.png"
#define YWR_RUN_R_FRAME		12
#define YWR_RUN_DL			"assets/img/yawara/yawara_run_down_left.png"
#define YWR_RUN_DL_FRAME	9
#define YWR_RUN_DR			"assets/img/yawara/yawara_idle_down_right.png"
#define YWR_RUN_DR_FRAME	1
#define YWR_RUN_UL			"assets/img/yawara/yawara_run_up_left.png"
#define YWR_RUN_UL_FRAME	9
#define YWR_RUN_UR			"assets/img/yawara/yawara_idle_up_right.png"
#define YWR_RUN_UR_FRAME	1

#define YWR_DEATH		"assets/penguin/img/penguindeath.png"
#define YWR_DEATH_FRAME	5
#define YWR_DEATH_SOUND	"assets/penguin/audio/boom.wav"

Yawara* Yawara::player;

Yawara::Yawara(GameObject& associated) : Component(associated) {
	player = this;

	Sprite* sp = new Sprite(associated, YWR_IDLE_R, YWR_IDLE_R_FRAME, YWR_ANI_TIME);
	associated.AddComponent(sp);
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);

	hp = YWR_HP;
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
			speed = {-YWR_SPEED/2, -YWR_SPEED/2};
			if (dir != LEFT_UP) {
				change_sprite = true;
			}
			dir = LEFT_UP;
		} else if (input.IsKeyDown(D_KEY)) {
			speed = {YWR_SPEED/2, -YWR_SPEED/2};
			if (dir != RIGHT_UP) {
				change_sprite = true;
			}
			dir = RIGHT_UP;
		} else {
			speed = {0, -YWR_SPEED};
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
			speed = {-YWR_SPEED/2, YWR_SPEED/2};
			if (dir != LEFT_DOWN) {
				change_sprite = true;
			}
			dir = LEFT_DOWN;
		} else if (input.IsKeyDown(D_KEY)) {
			speed = {YWR_SPEED/2, YWR_SPEED/2};
			if (dir != RIGHT_DOWN) {
				change_sprite = true;
			}
			dir = RIGHT_DOWN;
		} else {
			speed = {0, YWR_SPEED};
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
		speed = {-YWR_SPEED, 0};
		if (dir != LEFT) {
			change_sprite = true;
		}
		dir = LEFT;

		if (idle) {
			change_sprite = true;
		}
		idle = false;
	} else if (input.IsKeyDown(D_KEY)) {
		speed = {YWR_SPEED, 0};
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
		Vec2 position = associated.box.Center();
		change_sprite = false;
		if (idle) {	
			Sprite* sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));
			if (sp) {
				switch (dir) {
					case RIGHT:
						sp->Open(YWR_IDLE_R);
						sp->SetFrameCount(YWR_IDLE_R_FRAME);
					break;

					case LEFT:
						sp->Open(YWR_IDLE_L);
						sp->SetFrameCount(YWR_IDLE_L_FRAME);
					break;

					case UP:
						sp->Open(YWR_IDLE_U);
						sp->SetFrameCount(YWR_IDLE_U_FRAME);
					break;

					case DOWN:
						sp->Open(YWR_IDLE_D);
						sp->SetFrameCount(YWR_IDLE_D_FRAME);
					break;

					case RIGHT_UP:
						sp->Open(YWR_IDLE_UR);
						sp->SetFrameCount(YWR_IDLE_UR_FRAME);
					break;

					case RIGHT_DOWN:
						sp->Open(YWR_IDLE_DR);
						sp->SetFrameCount(YWR_IDLE_DR_FRAME);
					break;

					case LEFT_UP:
						sp->Open(YWR_IDLE_UL);
						sp->SetFrameCount(YWR_IDLE_UL_FRAME);
					break;

					case LEFT_DOWN:
						sp->Open(YWR_IDLE_DL);
						sp->SetFrameCount(YWR_IDLE_DL_FRAME);
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
						sp->Open(YWR_RUN_R);
						sp->SetFrameCount(YWR_RUN_R_FRAME);
					break;

					case LEFT:
						sp->Open(YWR_RUN_L);
						sp->SetFrameCount(YWR_RUN_L_FRAME);
					break;

					case UP:
						sp->Open(YWR_RUN_U);
						sp->SetFrameCount(YWR_RUN_U_FRAME);
					break;

					case DOWN:
						sp->Open(YWR_RUN_D);
						sp->SetFrameCount(YWR_RUN_D_FRAME);
					break;

					case RIGHT_UP:
						sp->Open(YWR_RUN_UR);
						sp->SetFrameCount(YWR_RUN_UR_FRAME);
					break;

					case RIGHT_DOWN:
						sp->Open(YWR_RUN_DR);
						sp->SetFrameCount(YWR_RUN_DR_FRAME);
					break;

					case LEFT_UP:
						sp->Open(YWR_RUN_UL);
						sp->SetFrameCount(YWR_RUN_UL_FRAME);
					break;

					case LEFT_DOWN:
						sp->Open(YWR_RUN_DL);
						sp->SetFrameCount(YWR_RUN_DL_FRAME);
					break;

					default:
					break;
				}
			}
		}
		associated.box.Centered(position);
	}

	associated.box.x += speed.x*dt;
	associated.box.y += speed.y*dt;

	if (hp <= 0) {
		associated.RequestDelete();
		Camera::Unfollow();

		GameObject *go = new GameObject();
		std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
		std::shared_ptr<GameObject> ptr = weak_ptr.lock();
		
		Sprite* sp = new Sprite(*ptr, YWR_DEATH, YWR_DEATH_FRAME, YWR_ANI_TIME, YWR_DEATH_FRAME * YWR_ANI_TIME);
		Sound *so = new Sound(*ptr, YWR_DEATH_SOUND);
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