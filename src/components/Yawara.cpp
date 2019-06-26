#include "Yawara.h"

#include "Game.h"
#include "Sprite.h"
#include "Collider.h"
#include "InputManager.h"
#include "Camera.h"
#include "Hitbox.h"
#include "Claw.h"
#include "Tongue.h"
#include "Sound.h"
#include "Tapu.h"
#include "Hitbox.h"

#define YWR_HP			100
#define YWR_SPEED		500
#define YWR_ATT			15
#define YWR_DEF			1

#define YWR_DGE_SPEED	2000
#define YWR_DGE_CD		0.5
#define YWR_DGE_ACT		0.2

#define YWR_ATK_CD		0.5
#define YWR_ATK_ACT		4*0.05

#define YWR_ANI_TIME	0.070

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
#define YWR_RUN_L_FRAME		9
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

	//sp->SetScale({0.7, 0.7});
	//cl->SetScale({0.7, 0.7});

	hp = YWR_HP;
	att = YWR_ATT;
	def = YWR_DEF;
	speed = {0, 0};
	dir = RIGHT;
	act = MOV;
	idle = true;
	hitTime.Restart();

	boostMap[HPBOOST] = {false, 1};
	boostMap[DEFBOOST] = {false, 1};
	boostMap[ATTBOOST] = {false, 1};
}

Yawara::~Yawara() {
	player = nullptr;
}

void Yawara::Start() {
	// Gera Tapu.
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
	dge_cd.Update(dt);
	atk_cd.Update(dt);
	hitTime.Update(dt);

	if(boostMap[HPBOOST].isBoosted){
		static Timer hpTimer;

		if(hpTimer.Get() == 0)
			hp = YWR_HP * boostMap[HPBOOST].factor;

		hpTimer.Update(dt);

		if(hpTimer.Get() >= 15){
			hpTimer.Restart();

			if(hp > YWR_HP)
				hp = YWR_HP;

			boostMap[HPBOOST] = {false, 1};
		}
	}
	if(boostMap[ATTBOOST].isBoosted){
		static Timer attTimer;

		if(attTimer.Get() == 0){
			att = YWR_ATT * boostMap[ATTBOOST].factor;
			std::shared_ptr<GameObject> tp_go = tapu.lock();
			if(tp_go){
				Tapu* tp =  static_cast<Tapu*> (tp_go->GetComponent("Tapu"));
				if(tp)
					tp->SetDamageFactor(boostMap[ATTBOOST].factor);
			}
		}

		attTimer.Update(dt);

		if(attTimer.Get() >= 15){
			attTimer.Restart();
			att = YWR_ATT;
			boostMap[ATTBOOST] = {false, 1};
			std::shared_ptr<GameObject> tp_go = tapu.lock();
			if(tp_go){
				Tapu* tp =  static_cast<Tapu*> (tp_go->GetComponent("Tapu"));
				if(tp)
					tp->SetDamageFactor(1);
			}
		}
	}
	if(boostMap[DEFBOOST].isBoosted){
		static Timer defTimer;

		if(defTimer.Get() == 0)
			def = YWR_DEF * boostMap[DEFBOOST].factor;

		defTimer.Update(dt);

		if(defTimer.Get() >= 15){
			defTimer.Restart();
			def = YWR_DEF;
			boostMap[DEFBOOST] = {false, 1};
		}
	}

	if (hp <= 0) {
		Camera::Unfollow();
		associated.RequestDelete();
	} else {
		Comand(dt);
		DoAction(dt);
	}
}

void Yawara::Render() {

}

bool Yawara::Is(std::string type) {
	return !strcmp(type.c_str(), "Yawara");
}

void Yawara::NotifyCollision(GameObject& other) {
	Hitbox *hitbox = static_cast<Hitbox *>(other.GetComponent("Hitbox"));
	
	if (hitbox && hitbox->targetsPlayer && hitTime.Get() >= HIT_COOL_DOWN)
	{
		float defended = def - 1;

		if(defended > 1)
			defended = 1;
		
		hp -= (1 - defended) * hitbox->GetDamage();
		hitTime.Restart();
	}
}

Vec2 Yawara::GetPos() {
	return {associated.box.x, associated.box.y};
}

Vec2 Yawara::GetCenterPos() {
	return associated.box.Center();
}

void Yawara::Comand(float dt) {
	InputManager& input = InputManager::GetInstance();

	switch (act) {
		case MOV:
			if (input.IsKeyDown(W_KEY)) {
				if (input.IsKeyDown(A_KEY)) {
					if (dir != UP_LEFT) {
						change_sprite = true;
					}
					dir = UP_LEFT;
				} else if (input.IsKeyDown(D_KEY)) {
					if (dir != UP_RIGHT) {
						change_sprite = true;
					}
					dir = UP_RIGHT;
				} else {
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
					if (dir != DOWN_LEFT) {
						change_sprite = true;
					}
					dir = DOWN_LEFT;
				} else if (input.IsKeyDown(D_KEY)) {
					if (dir != DOWN_RIGHT) {
						change_sprite = true;
					}
					dir = DOWN_RIGHT;
				} else {
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
				if ((dir != LEFT) || idle) {
					change_sprite = true;
				}
				dir = LEFT;
				idle = false;
			} else if (input.IsKeyDown(D_KEY)) {
				if ((dir != RIGHT) || idle) {
					change_sprite = true;
				}
				dir = RIGHT;
				idle = false;
			} else {
				if (!idle) {
					change_sprite = true;
				}
				idle = true;
			}

			if (input.KeyPress(SPACE_KEY)) {
				if (dge_cd.Get() > YWR_DGE_CD) {
					SetDge();
					associated.RemoveComponent(associated.GetComponent("Collider"));
					act = DGE;
					break;
				}
			}

			if (input.MousePress(RIGHT_MOUSE_BUTTON)) {
				if (atk_cd.Get() > YWR_ATK_CD) {
					SetAtk();
					act = ATK;
					break;
				}
			}
		break;

		case ATK:
			atk_act.Update(dt);
			if (atk_act.Get() > YWR_ATK_ACT) {
				act = MOV;

				Sprite* sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));
				sp->SetFrameTime(YWR_ANI_TIME);
				change_sprite = true;

				atk_act.Restart();
				atk_cd.Restart();
			}
		break;

		case DGE:
			dge_act.Update(dt);
			if (dge_act.Get() > YWR_DGE_ACT) {
				act = MOV;

				Sprite* sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));
				sp->SetFrameTime(YWR_ANI_TIME);
				Collider *cl = new Collider(associated);
				associated.AddComponent(cl);
				change_sprite = true;
				speed = {0, 0};
				associated.angleDeg = 0;

				dge_act.Restart();
				dge_cd.Restart();
			}
		break;

		default:
		break;
	}
}

void Yawara::DoAction(float dt) {
	switch (act) {
		case MOV:
			SetMov();
			associated.box.x += speed.x*dt;
			associated.box.y += speed.y*dt;
		break;

		case ATK:
		break;

		case DGE:
			associated.box.x += speed.x*dt;
			associated.box.y += speed.y*dt;
		break;

		default:
		break;
	}
}

void Yawara::SetMov() {
	if (change_sprite) {
		Vec2 position = associated.box.Center();
		change_sprite = false;
		if (idle) {	
			speed = {0, 0};
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

					case UP_RIGHT:
						sp->Open(YWR_IDLE_UR);
						sp->SetFrameCount(YWR_IDLE_UR_FRAME);
					break;

					case DOWN_RIGHT:
						sp->Open(YWR_IDLE_DR);
						sp->SetFrameCount(YWR_IDLE_DR_FRAME);
					break;

					case UP_LEFT:
						sp->Open(YWR_IDLE_UL);
						sp->SetFrameCount(YWR_IDLE_UL_FRAME);
					break;

					case DOWN_LEFT:
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
						speed = {YWR_SPEED, 0};
						sp->Open(YWR_RUN_R);
						sp->SetFrameCount(YWR_RUN_R_FRAME);
					break;

					case LEFT:
						speed = {-YWR_SPEED, 0};
						sp->Open(YWR_RUN_L);
						sp->SetFrameCount(YWR_RUN_L_FRAME);
					break;

					case DOWN:
						speed = {0, YWR_SPEED};
						sp->Open(YWR_RUN_D);
						sp->SetFrameCount(YWR_RUN_D_FRAME);
					break;

					case DOWN_RIGHT:
						speed = {YWR_SPEED/2, YWR_SPEED/2};
						sp->Open(YWR_RUN_DR);
						sp->SetFrameCount(YWR_RUN_DR_FRAME);
					break;

					case DOWN_LEFT:
						speed = {-YWR_SPEED/2, YWR_SPEED/2};
						sp->Open(YWR_RUN_DL);
						sp->SetFrameCount(YWR_RUN_DL_FRAME);
					break;

					case UP:
						speed = {0, -YWR_SPEED};
						sp->Open(YWR_RUN_U);
						sp->SetFrameCount(YWR_RUN_U_FRAME);
					break;

					case UP_RIGHT:
						speed = {YWR_SPEED/2, -YWR_SPEED/2};
						sp->Open(YWR_RUN_UR);
						sp->SetFrameCount(YWR_RUN_UR_FRAME);
					break;

					case UP_LEFT:
						speed = {-YWR_SPEED/2, -YWR_SPEED/2};
						sp->Open(YWR_RUN_UL);
						sp->SetFrameCount(YWR_RUN_UL_FRAME);
					break;

					default:
						speed = {0, 0};
					break;
				}
			}
		}
		associated.box.Centered(position);
	}
}

void Yawara::SetDge() {
	Vec2 position = associated.box.Center();
	
	Sprite* sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));
	if (sp) {
		sp->Open("assets/penguin/img/dash.png");
		sp->SetFrameCount(4);
		sp->SetFrameTime(0.04);
		switch (dir) {
			case RIGHT:
				speed = {YWR_DGE_SPEED, 0};
				associated.angleDeg = 0;
			break;

			case LEFT:
				speed = {-YWR_DGE_SPEED, 0};
				associated.angleDeg = 180;
			break;

			case DOWN:
				speed = {0, YWR_DGE_SPEED};
				associated.angleDeg = 90;
			break;

			case DOWN_RIGHT:
				speed = {YWR_DGE_SPEED/2, YWR_DGE_SPEED/2};
				associated.angleDeg = 45;
			break;

			case DOWN_LEFT:
				speed = {-YWR_DGE_SPEED/2, YWR_DGE_SPEED/2};
				associated.angleDeg = 135;
			break;

			case UP:
				speed = {0, -YWR_DGE_SPEED};
				associated.angleDeg = 270;
			break;

			case UP_RIGHT:
				speed = {YWR_DGE_SPEED/2, -YWR_DGE_SPEED/2};
				associated.angleDeg = 315;
			break;

			case UP_LEFT:
				speed = {-YWR_DGE_SPEED/2, -YWR_DGE_SPEED/2};
				associated.angleDeg = 225;
			break;

			default:
				speed = {0, 0};
			break;
		}
	}
	associated.box.Centered(position);
}

void Yawara::SetAtk() {
	GameObject *go = new GameObject();
	std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
	std::shared_ptr<GameObject> ptr = weak_ptr.lock();

	Sprite* sp = new Sprite(*ptr, "assets/penguin/img/slash.png", 5, 0.05, 5*0.05);
	Hitbox* hit = new Hitbox(*ptr);
	ptr->AddComponent(sp);
	ptr->AddComponent(hit);
	hit->colisor->SetScale({0.55, 0.85});
	hit->colisor->SetOffset({0, 0});
	ptr->box.Centered(associated.box.Center());

	switch (dir) {
		case RIGHT:
			ptr->box.x += 100;
			ptr->angleDeg = 0;
		break;

		case LEFT:
			ptr->box.x -= 100;
			ptr->angleDeg = 180;
		break;

		case DOWN:
			ptr->box.y += 100;
			ptr->angleDeg = 90;
		break;

		case DOWN_RIGHT:
			ptr->box.x += 50;
			ptr->box.y += 50;
			ptr->angleDeg = 45;
		break;

		case DOWN_LEFT:
			ptr->box.x -= 50;
			ptr->box.y += 50;
			ptr->angleDeg = 135;
		break;

		case UP:
			ptr->box.y -= 100;
			ptr->angleDeg = 270;
		break;

		case UP_RIGHT:
			ptr->box.x += 50;
			ptr->box.y -= 50;
			ptr->angleDeg = 315;
		break;

		case UP_LEFT:
			ptr->box.x -= 50;
			ptr->box.y -= 50;
			ptr->angleDeg = 225;
		break;

		default:
			std::cout << "Unknow direction to atack!\n";
		break;
	}
}

void Yawara::Boost(Boosts which, float factor){

	boostMap[which] = {true, factor};
}
