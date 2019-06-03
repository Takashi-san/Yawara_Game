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

int safeX;
int safeY;

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

	/*
	for (int i = 0; i < nMinions; i++){
		offset = (360/nMinions)*i;

		GameObject *go = new GameObject();
		weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
		minionArray.emplace_back(weak_ptr);
		ptr = weak_ptr.lock();

		Minion *mini = new Minion(*ptr, Game::GetInstance().GetCurrentState().GetObjectPtr(&associated), offset);
		ptr->box.x = 0;
		ptr->box.y = 0;
		ptr->AddComponent(mini);
	}
	*/
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

				destination = Yawara::player->GetPos();

				// Muda estado.
				state = RESTING;
				restTimer.Restart();
				break;
			}

			// Anda.

			if (speed.y > ALIEN_SPEED)
				safeY = associated.box.Center().y + SAFE_UP;
			else if (speed.y < -ALIEN_SPEED)
				safeY = associated.box.Center().y + SAFE_UP + SAFE_DOWN;
			else
				safeY = associated.box.Center().y + 2 * SAFE_DOWN;

			if (speed.x > ALIEN_SPEED)
				safeX = associated.box.Center().x + SAFE_SIDE;
			else if (speed.x < -ALIEN_SPEED)
				safeX = associated.box.Center().x - SAFE_SIDE;
			else
				safeX = associated.box.Center().x;

			moveAllowed = (Floor::loaded && Floor::AtAllowedArea(safeX, safeY, 0));
			if (!moveAllowed)
			{
				cout << "move to : " << safeX << ' ' << safeY << '\t' << speed.x << ' ' << speed.y << endl;
			}
			else
			{
				associated.box.x += speed.x * dt;
				associated.box.y += speed.y * dt;
			}
			break;

		case RESTING:
			restTimer.Update(dt);
			if (restTimer.Get() > ALIEN_REST_BASE + restOffset)
			{
				destination = Yawara::player->GetPos();
				if ((destination - associated.box.Center()).Modulo() != 0)
				{
					speed = ((destination - associated.box.Center()) / ((destination - associated.box.Center()).Modulo())) * ALIEN_SPEED;
				}
				else
				{
					speed = {0, 0};
				}

				state = MOVING;
				moveAllowed = 1;
			}
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