#include "Capelobo.h"
#include "Sprite.h"
#include "InputManager.h"
#include "Camera.h"
#include "State.h"
#include "Game.h"
#include "Minion.h"
#include "Collider.h"
#include "Bullet.h"
#include "Sound.h"
#include "Yawara.h"
#include "Floor.h"

Capelobo *Capelobo::boss;

bool moveAllowed = 1;

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

	state = RESTING;
}

void Capelobo::Start()
{
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;
	float offset;
}

Capelobo::~Capelobo()
{
	boss = nullptr;
}

void Capelobo::Update(float dt)
{
	float dst = 999999;

	associated.angleDeg += (ALIEN_VEL_ANG / 0.0174533) * dt;

	if (Yawara::player != nullptr)
	{
		int j;
		switch (state)
		{
		case MOVING:
			moveTimer.Update(dt);
			if (moveTimer.Get() < BOSS_MOVEMENT)
			{
				destination = Yawara::player->GetPos();

				if ((destination - associated.box.Center()).Modulo() != 0)
					speed = ((destination - associated.box.Center()) / ((destination - associated.box.Center()).Modulo())) * ALIEN_SPEED;
				else
					speed = {0, 0};

				// Se chegou no destino.
				if (((associated.box.Center().x <= destination.x + dt * abs(speed.x)) && (associated.box.Center().x >= destination.x - dt * abs(speed.x)) &&
					 (associated.box.Center().y <= destination.y + dt * abs(speed.y)) && (associated.box.Center().y >= destination.y - dt * abs(speed.y))) ||
					!moveAllowed)
				{
					// Coloca na posição final.
					if (moveAllowed)
						associated.box.Centered(destination);
					speed.x = 0;
					speed.y = 0;

					// Muda estado.
					state = RESTING;
					restTimer.Restart();
					break;
				}

				// Corrigindo rota pra andar em 45 graus
				if (abs(speed.x) > abs(speed.y) && (speed.y < -10 || speed.y > 10))
				{
					int signalY = speed.y / abs(speed.y);
					int signalX = speed.x / abs(speed.x);
					speed = {speed.x, signalY * signalX * speed.x};
					change_sprite = true;
				}
				else if (abs(speed.x) < abs(speed.y) && (speed.x < -10 || speed.x > 10))
				{
					int signalY = speed.y / abs(speed.y);
					int signalX = speed.x / abs(speed.x);
					speed = {signalY * signalX * speed.y, speed.y};
					change_sprite = true;
				}
				else
					change_sprite = true;

				// Seta direção para sprite

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

				// Verifica se pode andar

				// if (speed.y > 100)
				// {
				// 	safeY = associated.box.Center().y + SAFE_UP;
				// }
				// else if (speed.y < -100)
				// 	safeY = associated.box.Center().y + SAFE_UP + SAFE_DOWN;
				// else
				// 	safeY = associated.box.Center().y + 2 * SAFE_DOWN;

				// if (speed.x > 100)
				// 	safeX = associated.box.Center().x + SAFE_SIDE;
				// else if (speed.x < -100)
				// 	safeX = associated.box.Center().x - SAFE_SIDE;
				// else
				// 	safeX = associated.box.Center().x;

				// moveAllowed = (Floor::loaded && Floor::AtAllowedArea(safeX, safeY, 0));
				// cout << moveAllowed << endl;
				// if (!moveAllowed)
				// {
				// 	state = RESTING;
				// 	// cout << "move to : " << safeX << ' ' << safeY << '\t' << speed.x << ' ' << speed.y << "\t\t" << associated.box.Center().x << ' ' << associated.box.Center().y << endl;
				// }

				// Muda Sprite

				// Anda.

				associated.box.x += speed.x * dt;
				associated.box.y += speed.y * dt;
			}
			else
			{
				// Zera velocidade.
				speed.x = 0;
				speed.y = 0;

				// Muda estado.
				state = RESTING;
				restTimer.Restart();
			}
			break;

		case RESTING:
			restTimer.Update(dt);
			if (restTimer.Get() > ALIEN_REST_BASE + restOffset)
			{
				destination = Yawara::player->GetPos();
				if ((destination - associated.box.Center()).Modulo() != 0)
					speed = ((destination - associated.box.Center()) / ((destination - associated.box.Center()).Modulo())) * ALIEN_SPEED;
				else
					speed = {0, 0};

				temp_speed = speed;

				state = MOVING;
				moveAllowed = 1;
				moveTimer.Restart();
			}
			break;

		case BASIC_ATTACK:
			break;

		case LOAD_ATTACK:
			break;
		}
	}

	if (hp <= 0)
	{
		associated.RequestDelete();

		GameObject *go = new GameObject();
		std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
		std::shared_ptr<GameObject> ptr = weak_ptr.lock();

		Sprite *sp = new Sprite(*ptr, "assets/img/aliendeath.png", 4, 0.1, 4 * 0.1);
		Sound *so = new Sound(*ptr, "assets/audio/boom.wav");
		ptr->box.Centered(associated.box.Center());
		ptr->AddComponent(sp);
		ptr->AddComponent(so);

		so->Play(1);
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
			switch (dir)
			{
			case RIGHT:
				cout << "RIGHT" << dir << endl;
				// sp->Open("assets/img/yawara/yawara_idle_right.png");
				// sp->SetFrameCount(12);
				break;

			case LEFT:
				cout << "LEFT" << endl;
				// sp->Open("assets/img/yawara/yawara_idle_left.png");
				// sp->SetFrameCount(12);
				break;

			case UP:
				cout << "UP" << endl;
				// sp->Open("assets/img/yawara/yawara_idle_up.png");
				// sp->SetFrameCount(1);
				break;

			case DOWN:
				cout << "DOWN" << endl;
				// sp->Open("assets/img/yawara/yawara_idle_down.png");
				// sp->SetFrameCount(1);
				break;

			case RIGHT_UP:
				cout << "RIGHT_UP" << dir << endl;
				// sp->Open("assets/img/yawara/yawara_idle_up_right.png");
				// sp->SetFrameCount(1);
				break;

			case RIGHT_DOWN:
				cout << "RIGHT_DOWN" << endl;
				// sp->Open("assets/img/yawara/yawara_idle_down_right.png");
				// sp->SetFrameCount(1);
				break;

			case LEFT_UP:
				cout << "LEFT_UP" << endl;
				// sp->Open("assets/img/yawara/yawara_idle_up_left.png");
				// sp->SetFrameCount(1);
				break;

			case LEFT_DOWN:
				cout << "LEFT_DOWN" << endl;
				// sp->Open("assets/img/yawara/yawara_idle_down_left.png");
				// sp->SetFrameCount(1);
				break;

			default:
				break;
			}
		}
		else if (state == RESTING)
		{
			switch (dir)
			{
			case RIGHT:
				cout << "RIGHT" << dir << endl;
				// sp->Open("assets/img/yawara/yawara_idle_right.png");
				// sp->SetFrameCount(12);
				break;

			case LEFT:
				cout << "LEFT" << endl;
				// sp->Open("assets/img/yawara/yawara_idle_left.png");
				// sp->SetFrameCount(12);
				break;

			case UP:
				cout << "UP" << endl;
				// sp->Open("assets/img/yawara/yawara_idle_up.png");
				// sp->SetFrameCount(1);
				break;

			case DOWN:
				cout << "DOWN" << endl;
				// sp->Open("assets/img/yawara/yawara_idle_down.png");
				// sp->SetFrameCount(1);
				break;

			case RIGHT_UP:
				cout << "RIGHT_UP" << dir << endl;
				// sp->Open("assets/img/yawara/yawara_idle_up_right.png");
				// sp->SetFrameCount(1);
				break;

			case RIGHT_DOWN:
				cout << "RIGHT_DOWN" << endl;
				// sp->Open("assets/img/yawara/yawara_idle_down_right.png");
				// sp->SetFrameCount(1);
				break;

			case LEFT_UP:
				cout << "LEFT_UP" << endl;
				// sp->Open("assets/img/yawara/yawara_idle_up_left.png");
				// sp->SetFrameCount(1);
				break;

			case LEFT_DOWN:
				cout << "LEFT_DOWN" << endl;
				// sp->Open("assets/img/yawara/yawara_idle_down_left.png");
				// sp->SetFrameCount(1);
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
	return !strcmp(type.c_str(), "Alien");
}

void Capelobo::NotifyCollision(GameObject &other)
{
	if (other.GetComponent("Bullet") && !static_cast<Bullet *>(other.GetComponent("Bullet"))->targetsPlayer)
		hp -= static_cast<Bullet *>(other.GetComponent("Bullet"))->GetDamage();
}