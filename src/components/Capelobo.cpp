#include "Capelobo.h"
#include "Sprite.h"
#include "InputManager.h"
#include "Camera.h"
#include "State.h"
#include "Game.h"
#include "Collider.h"
#include "Bullet.h"
#include "Sound.h"
#include "Yawara.h"
#include "Floor.h"
#include "Claw.h"
#include "Tongue.h"

#define DISTANCE_Y 150
#define DISTANCE_X 150

#define B_ATTACK_H 300
#define B_ATTACK_W 90

#define L_ATTACK_H 20
#define L_ATTACK_W 20

// Movement sprites

const string MOVE_RIGHT 	 = "assets/img/capelobo/capelobo_idle_left.png";
const string MOVE_RIGHT_DOWN = "";
const string MOVE_DOWN		 = "";
const string MOVE_LEFT_DOWN	 = "";
const string MOVE_LEFT		 = "assets/img/capelobo/capelobo_animacao_correndo.png";
const string MOVE_LEFT_UP	 = "";
const string MOVE_UP		 = "";
const string MOVE_RIGHT_UP	 = "";

// Resting sprites

const string REST_RIGHT		 = "assets/img/capelobo/capelobo_idle_left.png";
const string REST_RIGHT_DOWN = "assets/img/capelobo/capelobo_idle_left.png";
const string REST_DOWN		 = "assets/img/capelobo/capelobo_idle_left.png";
const string REST_LEFT_DOWN	 = "assets/img/capelobo/capelobo_idle_left.png";
const string REST_LEFT		 = "assets/img/capelobo/capelobo_idle_left.png";
const string REST_LEFT_UP	 = "assets/img/capelobo/capelobo_idle_left.png";
const string REST_UP		 = "assets/img/capelobo/capelobo_idle_left.png";
const string REST_RIGHT_UP	 = "assets/img/capelobo/capelobo_idle_left.png";

// Attacking sprites

const string ATTACK_RIGHT		= "assets/img/capelobo/capelobo_attack_r.png";
const string ATTACK_RIGHT_DOWN	= "";
const string ATTACK_DOWN		= "";
const string ATTACK_LEFT_DOWN	= "";
const string ATTACK_LEFT		= "";
const string ATTACK_LEFT_UP		= "";
const string ATTACK_UP			= "";
const string ATTACK_RIGHT_UP	= "";

// Load attack sprites

const string LOAD_RIGHT		 = "";
const string LOAD_RIGHT_DOWN = "";
const string LOAD_DOWN		 = "";
const string LOAD_LEFT_DOWN	 = "";
const string LOAD_LEFT		 = "";
const string LOAD_LEFT_UP	 = "";
const string LOAD_UP		 = "";
const string LOAD_RIGHT_UP	 = "";

Capelobo *Capelobo::boss;

bool moveAllowed = 1;
bool startedAttack = 0;

Vec2 temp_speed;

Capelobo::Capelobo(GameObject &associated, float restOffset) : Component(associated)
{
	boss = this;

	hp = 70;
	speed.x = 0;
	speed.y = 0;
	this->restOffset = restOffset;
	//Sprite *sp = new Sprite(associated, "assets/img/alien.png");
	Sprite *sp = new Sprite(associated, "assets/img/capelobo/capelobo_idle_left.png", 8, 0.150);
	//Sprite* sp = new Sprite(associated, "assets/img/guara_r.png", 12, 0.140);
	Collider *cl = new Collider(associated);
	associated.AddComponent(sp);
	associated.AddComponent(cl);

	state = SLEEPING;
}

void Capelobo::Start()
{
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;
}

Capelobo::~Capelobo()
{
	boss = nullptr;
}

void Capelobo::Update(float dt)
{
	associated.angleDeg += (BOSS_VEL_ANG / (PI / 180)) * dt;

	if (Yawara::player != nullptr)
	{
		switch (state)
		{
		case MOVING:
			moveTimer.Update(dt);
			startedAttack = 0;
			if (moveTimer.Get() < BOSS_MOVEMENT && moveAllowed)
			{
				enemyPos = Yawara::player->GetPos();

				if ((enemyPos - associated.box.Center()).Modulo() != 0)
					speed = ((enemyPos - associated.box.Center()) / ((enemyPos - associated.box.Center()).Modulo())) * BOSS_SPEED;
				else
					speed = {0, 0};

				// Se chegou no destino.
				if (((associated.box.Center().x <= 150 + enemyPos.x + dt * abs(speed.x)) && (associated.box.Center().x >= -150 + enemyPos.x - dt * abs(speed.x)) &&
					 (associated.box.Center().y <= 80 + enemyPos.y + dt * abs(speed.y)) && (associated.box.Center().y >= -150 + enemyPos.y - dt * abs(speed.y))) ||
					!moveAllowed)
				{
					speed.x = 0;
					speed.y = 0;

					// Muda estado.
					state = BASIC_ATTACK;
					// state = LOAD_ATTACK;
					break;
				}

				// Corrigindo rota pra andar em 45 graus
				if (abs(speed.x) > abs(speed.y) && (speed.y < -10 || speed.y > 10))
				{
					int signalY = speed.y / abs(speed.y);
					int signalX = speed.x / abs(speed.x);
					speed = {speed.x, signalY * signalX * speed.x};
				}
				else if (abs(speed.x) < abs(speed.y) && (speed.x < -10 || speed.x > 10))
				{
					int signalY = speed.y / abs(speed.y);
					int signalX = speed.x / abs(speed.x);
					speed = {signalY * signalX * speed.y, speed.y};
				}

				// Seta direção para sprite

				Direction lastDir = dir;

				if (speed.y > 10)
				{
					if (speed.x > 10)
						dir = RIGHT_DOWN;
					else if (speed.x < -10)
						dir = LEFT_DOWN;
					else
						dir = DOWN;
				}
				else if (speed.y < -10)
				{
					if (speed.x > 10)
						dir = RIGHT_UP;
					else if (speed.x < -10)
						dir = LEFT_UP;
					else
						dir = UP;
				}
				else
				{
					if (speed.x > 10)
						dir = RIGHT;
					else if (speed.x < -10)
						dir = LEFT;
				}
				if (dir != lastDir)
					change_sprite = true;

				// Verifica se pode andar

				if (speed.y > 0.1)
				{
					safeY = associated.box.Center().y + SAFE_UP + SAFE_DOWN;
				}
				else if (speed.y < -0.1)
					safeY = associated.box.Center().y + SAFE_UP;
				else
					safeY = associated.box.Center().y + SAFE_DOWN;

				if (speed.x > 0.1)
					safeX = associated.box.Center().x + SAFE_SIDE;
				else if (speed.x < -0.1)
					safeX = associated.box.Center().x - SAFE_SIDE;
				else
					safeX = associated.box.Center().x;

				moveAllowed = (Floor::loaded && Floor::AtAllowedArea(safeX, safeY, 0));
				if (!moveAllowed)
				{
					state = RESTING;
					restTimer.Restart();
				}
				// Anda.
				else
				{
					associated.box.x += speed.x * dt;
					associated.box.y += speed.y * dt;
				}
			}
			else
			{
				// Zera velocidade.
				speed.x = 0;
				speed.y = 0;

				// Muda estado.
				state = LOAD_ATTACK;
				restTimer.Restart();
				moveTimer.Restart();
			}
			break;

		case RESTING:
			restTimer.Update(dt);
			if (!change_sprite)
				change_sprite = true;
			startedAttack = 0;
			if (restTimer.Get() > BOSS_REST_BASE + restOffset)
			{
				enemyPos = Yawara::player->GetPos();
				if ((enemyPos - associated.box.Center()).Modulo() != 0)
					speed = ((enemyPos - associated.box.Center()) / ((enemyPos - associated.box.Center()).Modulo())) * BOSS_SPEED;
				else
					speed = {0, 0};

				temp_speed = speed;

				state = MOVING;
				// state = LOAD_ATTACK;
				moveAllowed = 1;
				moveTimer.Restart();
			}
			break;

		case SLEEPING:
			enemyPos = Yawara::player->GetPos();

			if ((enemyPos - associated.box.Center()).Modulo() <= 450)
				state = MOVING;

			break;

		case BASIC_ATTACK:
			attackTimer.Update(dt);
			if (change_sprite)
				change_sprite = false;

			if (!startedAttack)
			{
				GameObject *clawGO = new GameObject();
				weak_ptr<GameObject> weak_claw1 = Game::GetInstance().GetCurrentState().AddObject(clawGO);
				shared_ptr<GameObject> shared_claw1 = weak_claw1.lock();
				Claw *theClaw1;

				GameObject *clawGO2 = new GameObject();
				weak_ptr<GameObject> weak_claw2 = Game::GetInstance().GetCurrentState().AddObject(clawGO2);
				shared_ptr<GameObject> shared_claw2 = weak_claw2.lock();
				Claw *theClaw2;

				startedAttack = 1;

				weak_ptr<GameObject> weak_Boss = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);

				int angle = (Yawara::player->GetPos() - associated.box.Center()).Inclination();
				if (angle < 0)
					angle += 360;
				angle = angle - angle % 45;

				shared_claw1->box.h = B_ATTACK_H;
				shared_claw1->box.w = B_ATTACK_W;

				shared_claw2->box.h = B_ATTACK_H + B_ATTACK_W / 2;
				shared_claw2->box.w = B_ATTACK_W;

				switch (dir)
				{
				case RIGHT:
					shared_claw1->box.Centered(associated.box.Center()+Vec2({DISTANCE_X,0}));
					break;

				case UP:
					swap(shared_claw1->box.h, shared_claw1->box.w);
					shared_claw1->box.Centered(associated.box.Center()-Vec2({0,DISTANCE_X}));
					break;

				case LEFT:
					shared_claw1->box.Centered(associated.box.Center()-Vec2({DISTANCE_X,0}));
					break;

				case DOWN:
					swap(shared_claw1->box.h, shared_claw1->box.w);
					shared_claw1->box.Centered(associated.box.Center()+Vec2({0,DISTANCE_X}));
					break;

				case LEFT_DOWN:
					swap(shared_claw2->box.h, shared_claw2->box.w);
					shared_claw2->box.Centered(associated.box.Center()+Vec2({-shared_claw1->box.w/4,DISTANCE_Y}));
					
					theClaw2 = new Claw(*shared_claw2, CLAW_DAMAGE,true);
					shared_claw2->AddComponent(theClaw2);

					shared_claw1->box.Centered(associated.box.Center()-Vec2({DISTANCE_X,0}));
					shared_claw1->box.h -= shared_claw2->box.h/2;
					break;

				case LEFT_UP:
					swap(shared_claw2->box.h, shared_claw2->box.w);
					shared_claw2->box.Centered(associated.box.Center()-Vec2({shared_claw1->box.w/4,DISTANCE_Y}));
					
					theClaw2 = new Claw(*shared_claw2, CLAW_DAMAGE,true);
					shared_claw2->AddComponent(theClaw2);

					shared_claw1->box.Centered(associated.box.Center()-Vec2({DISTANCE_X,-shared_claw2->box.h/2}));
					shared_claw1->box.h -= shared_claw2->box.h/2;
					break;

				case RIGHT_DOWN:
					swap(shared_claw2->box.h, shared_claw2->box.w);
					shared_claw2->box.Centered(associated.box.Center()+Vec2({shared_claw1->box.w/4,DISTANCE_Y}));
					
					theClaw2 = new Claw(*shared_claw2, CLAW_DAMAGE,true);
					shared_claw2->AddComponent(theClaw2);
					
					shared_claw1->box.Centered(associated.box.Center()+Vec2({DISTANCE_X,0}));
					shared_claw1->box.h -= shared_claw2->box.h/2;
					break;

				case RIGHT_UP:
					swap(shared_claw2->box.h, shared_claw2->box.w);
					shared_claw2->box.Centered(associated.box.Center()+Vec2({shared_claw1->box.w/4,-DISTANCE_Y}));
					
					theClaw2 = new Claw(*shared_claw2, CLAW_DAMAGE,true);
					shared_claw2->AddComponent(theClaw2);

					shared_claw1->box.Centered(associated.box.Center()+Vec2({DISTANCE_X,shared_claw2->box.h/2}));
					shared_claw1->box.h -= shared_claw2->box.h/2;
					break;
				default:
					break;
				}

				theClaw1 = new Claw(*shared_claw1, CLAW_DAMAGE, true);

				shared_claw1->AddComponent(theClaw1);
				if(shared_claw1 != nullptr)
					change_sprite = true;
			}

			if (attackTimer.Get() > 1)
			{
				state = RESTING;
				attackTimer.Restart();
				restTimer.Restart();
			}
			break;
		
		case LOAD_ATTACK:
			
			attackTimer.Update(dt);
			if (change_sprite)
				change_sprite = false;

			if (!startedAttack)
			{
				startedAttack = 1;

				GameObject *tongueGO = new GameObject();
				weak_ptr<GameObject> weak_tongue = Game::GetInstance().GetCurrentState().AddObject(tongueGO);
				shared_ptr<GameObject> shared_tongue = weak_tongue.lock();

				shared_tongue->box.w = L_ATTACK_W;
				shared_tongue->box.h = L_ATTACK_H;
				shared_tongue->box.x = associated.box.Center().x - shared_tongue->box.w / 2;
				shared_tongue->box.y = associated.box.Center().y;

				int angle = (Yawara::player->GetPos() - associated.box.Center()).Inclination();
				if (angle < 0)
					angle += 360;

				angle = angle - angle % 45;

				Tongue *theTongue = new Tongue(*shared_tongue, TONGUE_DAMAGE, TONGUE_SPEED, angle, TONGUE_MAX_DIST, true);

				shared_tongue->AddComponent(theTongue);
				if(shared_tongue != nullptr)
					change_sprite = true;
			}

			if (attackTimer.Get() > 1.5)
			{
				state = MOVING;
				attackTimer.Restart();
				restTimer.Restart();
			}
			break;
		}
	}

	if (hp <= 0)
	{
		associated.RequestDelete();
		/*
		GameObject *go = new GameObject();
		std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
		std::shared_ptr<GameObject> ptr = weak_ptr.lock();

		Sprite *sp = new Sprite(*ptr, "assets/img/aliendeath.png", 4, 0.1, 4 * 0.1);
		Sound *so = new Sound(*ptr, "assets/audio/boom.wav");
		ptr->box.Centered(associated.box.Center());
		ptr->AddComponent(sp);
		ptr->AddComponent(so);

		so->Play(1);
		*/
	}
}

void Capelobo::Render()
{
	Vec2 position = associated.box.Center();

	Sprite *sp = static_cast<Sprite *>(associated.GetComponent("Sprite"));
	if (change_sprite && sp)
	{
		change_sprite = false;
		if (state == MOVING)
		{
			sp->SetFrameTime(0.1);
			switch (dir)
			{
			case RIGHT:
				sp->Open(MOVE_RIGHT);
				sp->SetFrameCount(8);
				break;

			case LEFT:
				sp->Open(MOVE_LEFT);
				sp->SetFrameCount(12);
				break;

			case UP:
				// sp->Open(MOVE_UP);
				// sp->SetFrameCount(1);
				break;

			case DOWN:
				// sp->Open(MOVE_DOWN);
				// sp->SetFrameCount(1);
				break;

			case RIGHT_UP:
				// sp->Open(MOVE_RIGHT_UP);
				// sp->SetFrameCount(1);
				break;

			case RIGHT_DOWN:
				// sp->Open(MOVE_RIGHT_DOWN);
				// sp->SetFrameCount(1);
				break;

			case LEFT_UP:
				// sp->Open(MOVE_LEFT_UP);
				// sp->SetFrameCount(1);
				break;

			case LEFT_DOWN:
				// sp->Open(MOVE_LEFT_DOWN);
				// sp->SetFrameCount(1);
				break;

			default:
				break;
			}
		}
		else if (state == RESTING)
		{
			sp->SetFrameTime(0.15);
			switch (dir)
			{
			case RIGHT:
				sp->Open(REST_RIGHT);
				sp->SetFrameCount(8);
				break;

			case LEFT:
				sp->Open(REST_LEFT);
				sp->SetFrameCount(8);
				break;

			case UP:
				sp->Open(REST_UP);
				sp->SetFrameCount(8);
				break;

			case DOWN:
				sp->Open(REST_DOWN);
				sp->SetFrameCount(8);
				break;

			case RIGHT_UP:
				sp->Open(REST_RIGHT_UP);
				sp->SetFrameCount(8);
				break;

			case RIGHT_DOWN:
				sp->Open(REST_RIGHT_DOWN);
				sp->SetFrameCount(8);
				break;

			case LEFT_UP:
				sp->Open(REST_LEFT_UP);
				sp->SetFrameCount(8);
				break;

			case LEFT_DOWN:
				sp->Open(REST_LEFT_DOWN);
				sp->SetFrameCount(8);
				break;

			default:
				break;
			}
		}
		else if (state == BASIC_ATTACK)
		{
			sp->SetFrameTime(0.05);
			switch (dir)
			{
			case RIGHT:
				sp->Open(ATTACK_RIGHT);
				sp->SetFrameCount(16);
				break;

			case LEFT:
				// sp->Open(ATTACK_LEFT);
				// sp->SetFrameCount(16);
				break;

			case UP:
				// sp->Open(ATTACK_UP);
				// sp->SetFrameCount(16);
				break;

			case DOWN:
				// sp->Open(ATTACK_DOWN);
				// sp->SetFrameCount(16);
				break;

			case RIGHT_UP:
				// sp->Open(ATTACK_RIGHT_UP);
				// sp->SetFrameCount(16);
				break;

			case RIGHT_DOWN:
				// sp->Open(ATTACK_RIGHT_DOWN);
				// sp->SetFrameCount(16);
				break;

			case LEFT_UP:
				// sp->Open(ATTACK_LEFT_UP);
				// sp->SetFrameCount(16);
				break;

			case LEFT_DOWN:
				// sp->Open(ATTACK_LEFT_DOWN);
				// sp->SetFrameCount(16);
				break;

			default:
				break;
			}
		}
		else if (state == LOAD_ATTACK)
		{
			sp->SetFrameTime(0.05);
			switch (dir)
			{
			case RIGHT:
				// sp->Open(LOAD_RIGHT);
				// sp->SetFrameCount(16);
				break;

			case LEFT:
				// sp->Open(LOAD_LEFT);
				// sp->SetFrameCount(16);
				break;

			case UP:
				// sp->Open(LOAD_UP);
				// sp->SetFrameCount(16);
				break;

			case DOWN:
				// sp->Open(LOAD_DOWN);
				// sp->SetFrameCount(16);
				break;

			case RIGHT_UP:
				// sp->Open(LOAD_RIGHT_UP);
				// sp->SetFrameCount(16);
				break;

			case RIGHT_DOWN:
				// sp->Open(LOAD_RIGHT_DOWN);
				// sp->SetFrameCount(16);
				break;

			case LEFT_UP:
				// sp->Open(LOAD_LEFT_UP);
				// sp->SetFrameCount(16);
				break;

			case LEFT_DOWN:
				// sp->Open(LOAD_LEFT_DOWN);
				// sp->SetFrameCount(16);
				break;

			default:
				break;
			}
		}
	}
	associated.box.Centered(position);
}

bool Capelobo::Is(std::string type)
{
	return !strcmp(type.c_str(), "Capelobo");
}

void Capelobo::NotifyCollision(GameObject &other)
{
	if (other.GetComponent("Bullet") && !static_cast<Bullet *>(other.GetComponent("Bullet"))->targetsPlayer)
		hp -= static_cast<Bullet *>(other.GetComponent("Bullet"))->GetDamage();
}