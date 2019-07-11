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

#define YWR_HOWL_CD		0.5

#define YWR_SIT_GO		5
#define YWR_SIT_FRAME	4
#define YWR_SIT_TIME	0.1
#define YWR_SIT_R 		"assets/img/yawara/yawara_sit_right.png"
#define YWR_SIT_L 		"assets/img/yawara/yawara_sit_left.png"

#define YWR_IDLE_FRAME		5
#define YWR_IDLE_FRAME_LR	4
#define YWR_IDLE_FRAME_D	6
#define YWR_IDLE_TIME		0.1
#define YWR_IDLE_D			"assets/img/yawara/yawara_idle_down.png"
#define YWR_IDLE_U			"assets/img/yawara/yawara_idle_up.png"
#define YWR_IDLE_L			"assets/img/yawara/yawara_idle_left.png"
#define YWR_IDLE_R			"assets/img/yawara/yawara_idle_right.png"
#define YWR_IDLE_DL			"assets/img/yawara/yawara_idle_down_left.png"
#define YWR_IDLE_DR			"assets/img/yawara/yawara_idle_down_right.png"
#define YWR_IDLE_UL			"assets/img/yawara/yawara_idle_up_left.png"
#define YWR_IDLE_UR			"assets/img/yawara/yawara_idle_up_right.png"

#define YWR_RUN_FRAME		9
#define YWR_RUN_TIME		0.07
#define YWR_RUN_D			"assets/img/yawara/yawara_run_down.png"
#define YWR_RUN_U			"assets/img/yawara/yawara_run_up.png"
#define YWR_RUN_L			"assets/img/yawara/yawara_run_left.png"
#define YWR_RUN_R			"assets/img/yawara/yawara_run_right.png"
#define YWR_RUN_DL			"assets/img/yawara/yawara_run_down_left.png"
#define YWR_RUN_DR			"assets/img/yawara/yawara_run_down_right.png"
#define YWR_RUN_UL			"assets/img/yawara/yawara_run_up_left.png"
#define YWR_RUN_UR			"assets/img/yawara/yawara_run_up_right.png"

#define YWR_BITE_FX_FRAME	8
#define YWR_BITE_FX_TIME	0.05
#define YWR_BITE_FX			"assets/img/yawara/yawara_bite.png"
#define YWR_BITE_FX_RADIUS	100

#define YWR_BITE_SOUND		"assets/audio/sons/yawara/yawara_bite.ogg"
#define YWR_BITE_FRAME		5
#define YWR_BITE_TIME		0.07
#define YWR_BITE_D			"assets/img/yawara/yawara_bite_down.png"
#define YWR_BITE_U			"assets/img/yawara/yawara_bite_up.png"
#define YWR_BITE_L			"assets/img/yawara/yawara_bite_left.png"
#define YWR_BITE_R			"assets/img/yawara/yawara_bite_right.png"
#define YWR_BITE_DL			"assets/img/yawara/yawara_bite_down_left.png"
#define YWR_BITE_DR			"assets/img/yawara/yawara_bite_down_right.png"
#define YWR_BITE_UL			"assets/img/yawara/yawara_bite_up_left.png"
#define YWR_BITE_UR			"assets/img/yawara/yawara_bite_up_right.png"

#define YWR_HOWL_FRAME		5
#define YWR_HOWL_TIME		0.07
#define YWR_HOWL_EXTRA		1
#define YWR_HOWL_D			
#define YWR_HOWL_U			
#define YWR_HOWL_L			"assets/img/yawara/yawara_howl_left.png"
#define YWR_HOWL_R			"assets/img/yawara/yawara_howl_right.png"
#define YWR_HOWL_DL			
#define YWR_HOWL_DR			
#define YWR_HOWL_UL			
#define YWR_HOWL_UR			

#define YWR_DEATH_FRAME		4
#define YWR_DEATH_TIME		0.1
#define YWR_DEATH_EXTRA		2
#define YWR_DEATH_R			"assets/img/yawara/yawara_death_right.png"
#define YWR_DEATH_L			"assets/img/yawara/yawara_death_left.png"
#define YWR_DEATH_SOUND		"assets/penguin/audio/boom.wav"

Yawara* Yawara::player;

Yawara::Yawara(GameObject& associated) : Component(associated) {
	player = this;

	Sprite* sp = new Sprite(associated, YWR_IDLE_R, YWR_IDLE_FRAME, YWR_RUN_TIME);
	associated.AddComponent(sp);
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);

	bite = new Sound(associated, YWR_BITE_SOUND);
	associated.AddComponent(bite);

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

	/*
	// Gera sombra;
	GameObject* go = new GameObject();
	shadow_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
	std::shared_ptr<GameObject> ptr = shadow_ptr.lock();

	shadow = new Sprite(*ptr, YWR_SHADOW_LR);
	ptr->box.Centered(associated.box.Center());
	ptr->AddComponent(shadow);
	*/
}

Yawara::~Yawara() {
	player = nullptr;
}

void Yawara::Start() {
	// Gera efeito uivo.
	GameObject* go = new GameObject();
	std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
	std::shared_ptr<GameObject> ptr = weak_ptr.lock();

	howl = new Howl(*ptr);
	ptr->AddComponent(howl);

	// Gera Tapu.
	go = new GameObject();
	weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
	ptr = weak_ptr.lock();
	tapu = weak_ptr;

	Tapu* tp = new Tapu(*ptr, Game::GetInstance().GetCurrentState().GetObjectPtr(&associated));
	ptr->AddComponent(tp);
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

void Yawara::Boost(Boosts which, float factor){
	boostMap[which] = {true, factor};
}

Vec2 Yawara::GetPos() {
	return {associated.box.x, associated.box.y};
}

Vec2 Yawara::GetCenterPos() {
	return associated.box.Center();
}

int Yawara::GetHP() {
	return hp;
}

int Yawara::GetMaxHP() {
	return YWR_HP;
}

int Yawara::GetDirection() {
	return dir;
}

void Yawara::Update(float dt) {
	// Update cooldowns.
	dge_cd.Update(dt);
	atk_cd.Update(dt);
	howl_cd.Update(dt);
	hitTime.Update(dt);

	// Boosters.
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

	// Act or dead.
	if (hp <= 0) {
		static bool flag = true;

		if (flag) {
			flag = false;
			Camera::Unfollow();
			
			Vec2 pos = associated.box.Center();
			Sprite* sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));
			if (sp) {
				switch (dir) {
					case RIGHT:
					case UP:
					case UP_RIGHT:
					case DOWN_RIGHT:
						sp->Open(YWR_DEATH_R);
					break;

					case LEFT:
					case DOWN:
					case DOWN_LEFT:
					case UP_LEFT:
						sp->Open(YWR_DEATH_L);
					break;

					default:
					break;
				}
				sp->SetFrameCount(YWR_DEATH_FRAME);
				sp->SetFrameTime(YWR_DEATH_TIME);
				sp->SetStopFrame(YWR_DEATH_FRAME - 1);
				sp->SetSelfDestruct((YWR_DEATH_FRAME * YWR_DEATH_TIME) + YWR_DEATH_EXTRA);
			}
			associated.box.Centered(pos);
		}
	} else {
		Comand(dt);
		DoAction(dt);
	}
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

			if (input.KeyPress(E_KEY)) {
				if (howl_cd.Get() > YWR_HOWL_CD) {
					SetHowl();
					act = HOWL;
					break;
				}
			}
		break;

		case ATK:
			atk_act.Update(dt);
			if (atk_act.Get() > YWR_BITE_FX_TIME * YWR_BITE_FX_FRAME) {
				act = MOV;

				// Change back sprite.
				Sprite* sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));
				if (sp) {
					sp->SetFrameTime(YWR_RUN_TIME);
				}
				change_sprite = true;

				// Reset attack.
				atk_act.Restart();
				atk_cd.Restart();
			}
		break;

		case DGE:
			dge_act.Update(dt);
			if (dge_act.Get() > YWR_DGE_ACT) {
				act = MOV;

				// Change back sprite.
				Sprite* sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));
				if (sp) {
					sp->SetFrameTime(YWR_RUN_TIME);
				}
				change_sprite = true;

				// Return collider.
				Collider *cl = new Collider(associated);
				associated.AddComponent(cl);

				// Reset atributes.
				speed = {0, 0};
				associated.angleDeg = 0;

				// Reset dodge.
				dge_act.Restart();
				dge_cd.Restart();
			}
		break;

		case HOWL:
			howl_act.Update(dt);
			if (howl_act.Get() > YWR_HOWL_TIME * YWR_HOWL_FRAME + YWR_HOWL_EXTRA) {
				act = MOV;

				// Change back sprite.
				Sprite* sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));
				if (sp) {
					sp->SetFrameTime(YWR_RUN_TIME);
				}
				change_sprite = true;

				// Reset howl.
				howl_act.Restart();
				howl_cd.Restart();
			}
		break;

		default:
		break;
	}
}

void Yawara::DoAction(float dt) {
	static Timer sit;
	static bool good_boy;
	Vec2 pos;

	switch (act) {
		case MOV:
			if (idle) {
				sit.Update(dt);
				if ((sit.Get() > YWR_SIT_GO) && !good_boy) {
					pos = associated.box.Center();
					Sprite* sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));
					if (sp) {
						switch (dir) {
							case RIGHT:
							case UP:
							case UP_RIGHT:
							case DOWN_RIGHT:
								sp->Open(YWR_SIT_R);
							break;

							case LEFT:
							case DOWN:
							case DOWN_LEFT:
							case UP_LEFT:
								sp->Open(YWR_SIT_L);
							break;

							default:
							break;
						}
						sp->SetFrameCount(YWR_SIT_FRAME);
						sp->SetFrameTime(YWR_SIT_TIME);
						sp->SetStopFrame(YWR_SIT_FRAME - 1);
					}
					associated.box.Centered(pos);
					sit.Restart();
					good_boy = true;
				}
			} else {
				sit.Restart();
				good_boy = false;
			}

			SetMov();
			associated.box.x += speed.x*dt;
			associated.box.y += speed.y*dt;
		break;

		case ATK:
			sit.Restart();
			good_boy = false;
		break;

		case DGE:
			sit.Restart();
			good_boy = false;

			associated.box.x += speed.x*dt;
			associated.box.y += speed.y*dt;
		break;

		case HOWL:
			sit.Restart();
			good_boy = false;
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
						sp->SetFrameCount(YWR_IDLE_FRAME_LR);
					break;

					case LEFT:
						sp->Open(YWR_IDLE_L);
						sp->SetFrameCount(YWR_IDLE_FRAME_LR);
					break;

					case UP:
						sp->Open(YWR_IDLE_U);
						sp->SetFrameCount(YWR_IDLE_FRAME);
					break;

					case DOWN:
						sp->Open(YWR_IDLE_D);
						sp->SetFrameCount(YWR_IDLE_FRAME_D);
					break;

					case UP_RIGHT:
						sp->Open(YWR_IDLE_UR);
						sp->SetFrameCount(YWR_IDLE_FRAME);
					break;

					case DOWN_RIGHT:
						sp->Open(YWR_IDLE_DR);
						sp->SetFrameCount(YWR_IDLE_FRAME_D);
					break;

					case UP_LEFT:
						sp->Open(YWR_IDLE_UL);
						sp->SetFrameCount(YWR_IDLE_FRAME);
					break;

					case DOWN_LEFT:
						sp->Open(YWR_IDLE_DL);
						sp->SetFrameCount(YWR_IDLE_FRAME_D);
					break;

					default:
					break;
				}
				sp->SetFrameTime(YWR_IDLE_TIME);
			}
		} else {
			Sprite* sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));
			if (sp) {
				switch (dir) {
					case RIGHT:
						speed = {YWR_SPEED, 0};
						sp->Open(YWR_RUN_R);
					break;

					case LEFT:
						speed = {-YWR_SPEED, 0};
						sp->Open(YWR_RUN_L);
					break;

					case DOWN:
						speed = {0, YWR_SPEED};
						sp->Open(YWR_RUN_D);
					break;

					case DOWN_RIGHT:
						speed = {YWR_SPEED/2, YWR_SPEED/2};
						sp->Open(YWR_RUN_DR);
					break;

					case DOWN_LEFT:
						speed = {-YWR_SPEED/2, YWR_SPEED/2};
						sp->Open(YWR_RUN_DL);
					break;

					case UP:
						speed = {0, -YWR_SPEED};
						sp->Open(YWR_RUN_U);
					break;

					case UP_RIGHT:
						speed = {YWR_SPEED/2, -YWR_SPEED/2};
						sp->Open(YWR_RUN_UR);
					break;

					case UP_LEFT:
						speed = {-YWR_SPEED/2, -YWR_SPEED/2};
						sp->Open(YWR_RUN_UL);
					break;

					default:
						speed = {0, 0};
					break;
				}
				sp->SetFrameCount(YWR_RUN_FRAME);
				sp->SetFrameTime(YWR_RUN_TIME);
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
	// Create attack.
	GameObject *go = new GameObject();
	std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
	std::shared_ptr<GameObject> ptr = weak_ptr.lock();

	Sprite* sp = new Sprite(*ptr, YWR_BITE_FX, YWR_BITE_FX_FRAME, YWR_BITE_FX_TIME, YWR_BITE_FX_FRAME * YWR_BITE_FX_TIME);
	Hitbox* hit = new Hitbox(*ptr);
	ptr->AddComponent(sp);
	ptr->AddComponent(hit);
	hit->colisor->SetScale({0.50, 0.85});
	hit->colisor->SetOffset({0, 0});
	hit->SetDamage(att);
	hit->hitDie = false;
	ptr->box.Centered(associated.box.Center());

	// Get yawara sprite.
	sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));

	// Set offset, rotation and yawara sprite.
	if (sp) {
		Vec2 position = associated.box.Center();
		sp->SetFrameCount(YWR_BITE_FRAME);
		sp->SetFrameTime(YWR_BITE_TIME);
		sp->SetStopFrame(2);
		switch (dir) {
			case RIGHT:
				ptr->box.x += YWR_BITE_FX_RADIUS;
				ptr->angleDeg = 180;

				sp->Open(YWR_BITE_R);
			break;

			case LEFT:
				ptr->box.x -= YWR_BITE_FX_RADIUS;
				ptr->angleDeg = 0;

				sp->Open(YWR_BITE_L);
			break;

			case DOWN:
				ptr->box.y += YWR_BITE_FX_RADIUS;
				ptr->angleDeg = 270;

				sp->Open(YWR_BITE_D);
			break;

			case DOWN_RIGHT:
				ptr->box.x += YWR_BITE_FX_RADIUS * 0.666;
				ptr->box.y += YWR_BITE_FX_RADIUS * 0.666;
				ptr->angleDeg = 225;

				sp->Open(YWR_BITE_DR);
			break;

			case DOWN_LEFT:
				ptr->box.x -= YWR_BITE_FX_RADIUS * 0.666;
				ptr->box.y += YWR_BITE_FX_RADIUS * 0.666;
				ptr->angleDeg = 315;

				sp->Open(YWR_BITE_DL);
			break;

			case UP:
				ptr->box.y -= YWR_BITE_FX_RADIUS;
				ptr->angleDeg = 90;

				sp->Open(YWR_BITE_U);
			break;

			case UP_RIGHT:
				ptr->box.x += YWR_BITE_FX_RADIUS * 0.666;
				ptr->box.y -= YWR_BITE_FX_RADIUS * 0.666;
				ptr->angleDeg = 135;

				sp->Open(YWR_BITE_UR);
			break;

			case UP_LEFT:
				ptr->box.x -= YWR_BITE_FX_RADIUS * 0.666;
				ptr->box.y -= YWR_BITE_FX_RADIUS * 0.666;
				ptr->angleDeg = 45;

				sp->Open(YWR_BITE_UL);
			break;

			default:
				std::cout << "Unknow direction to attack!\n";
			break;
		}
		associated.box.Centered(position);
	}

	bite->Play();
}

void Yawara::SetHowl() {
	// Get yawara sprite.
	Sprite* sp = static_cast<Sprite*>(associated.GetComponent("Sprite"));

	// Set yawara sprite.
	if (sp) {
		Vec2 position = associated.box.Center();
		sp->SetFrameCount(YWR_HOWL_FRAME);
		sp->SetFrameTime(YWR_HOWL_TIME);
		sp->SetStopFrame(YWR_HOWL_FRAME - 1);
		switch (dir) {
			case RIGHT:
			case DOWN:
			case DOWN_RIGHT:
			case UP_RIGHT:
				sp->Open(YWR_HOWL_R);
			break;

			case LEFT:
			case UP:
			case DOWN_LEFT:
			case UP_LEFT:
				sp->Open(YWR_HOWL_L);
			break;

			default:
				std::cout << "Unknow direction to attack!\n";
			break;
		}
		associated.box.Centered(position);
	}

	// Set howl effect animation.
	howl->SetHowl(YWR_HOWL_TIME * YWR_HOWL_FRAME, YWR_HOWL_EXTRA);
}
