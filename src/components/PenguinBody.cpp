#include "PenguinBody.h"
#include "Game.h"
#include "Sprite.h"
#include "InputManager.h"
#include "PenguinCannon.h"
#include "Collider.h"
#include "Camera.h"
#include "Bullet.h"
#include "Sound.h"
#include "Alien.h"

PenguinBody *PenguinBody::player;

PenguinBody::PenguinBody(GameObject &associated) : Component(associated)
{
	player = this;

	//Sprite* sp = new Sprite(associated, "assets/img/penguin.png");
	Sprite *sp = new Sprite(associated, "assets/img/guara_r.png", 12, 0.070);
	associated.AddComponent(sp);
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);

	sp->SetScale(0.5, 0.5);

	linearSpeed = 0;
	angle = 0;
	hp = 100;
	speed.x = 0;
	speed.y = 0;
}

PenguinBody::~PenguinBody()
{
	player = nullptr;
}

void PenguinBody::Start()
{
	GameObject *go = new GameObject();
	std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
	std::shared_ptr<GameObject> ptr = weak_ptr.lock();
	pcannon = weak_ptr;

	PenguinCannon *pcan = new PenguinCannon(*ptr, Game::GetInstance().GetCurrentState().GetObjectPtr(&associated));
	ptr->box.x = associated.box.x;
	ptr->box.y = associated.box.y;
	ptr->AddComponent(pcan);
}

void PenguinBody::Update(float dt)
{
	InputManager &input = InputManager::GetInstance();

	Floor *theFloor = static_cast<Floor *>(associated.GetComponent("Floor"));

	if (input.IsKeyDown(W_KEY))
	{
		linearSpeed += PBODY_ACCEL * dt;
		if (linearSpeed > PBODY_VEL_CAP)
		{
			linearSpeed = PBODY_VEL_CAP;
		}
	}
	else if (input.IsKeyDown(S_KEY))
	{
		linearSpeed -= PBODY_ACCEL * dt;
		if (linearSpeed < -PBODY_VEL_CAP)
		{
			linearSpeed = -PBODY_VEL_CAP;
		}
	}

	else
	{
		if (linearSpeed <= 0.2 && linearSpeed >= -0.2)
			linearSpeed = 0.0;
		else
		{
			int speed_signal = linearSpeed / abs(linearSpeed);

			linearSpeed -= speed_signal * 2 * (PBODY_ACCEL * dt);
		}
	}

	if (theFloor != nullptr)
	{
		if (!theFloor->AtAllowedArea(front.x, front.y, 0) && linearSpeed > 0.1)
		{
			cout << "HIT THE WALL YOU DUMB DUMB WWWWWWWWWW at: " << associated.box.Center().x + 2 << '\t' << associated.box.Center().y << endl;
			// linearSpeed = -linearSpeed;
		}
		else if (!theFloor->AtAllowedArea(back.x, back.y, 0) && linearSpeed < -0.1)
		{
			cout << "HIT THE WALL YOU DUMB DUMB" << endl;
			// linearSpeed = -linearSpeed;
		}
		else
			cout << "Your safe " << associated.box.x << '\t' << associated.box.y << endl;
	}

	if (input.KeyPress(A_KEY))
	{
		angle -= 45 * 0.0174533;
	}
	if (input.KeyPress(D_KEY))
	{
		angle += 45 * 0.0174533;
	}
	if (angle >= 360 * 0.0174533 || angle <= -360 * 0.0174533)
	{
		angle = 0;
	}
	speed.x = linearSpeed;
	speed.y = 0;
	speed.Rotate(angle);
	associated.box.x += speed.x * dt;
	associated.box.y += speed.y * dt;
	associated.angleDeg = angle / 0.0174533;

	front = {(associated.box.Center().x + (5 * tileSize)) / tileSize, (associated.box.Center().y - (5 * tileSize)) / tileSize};
	back = {(associated.box.Center().x - associated.box.w - (5 * tileSize)) / tileSize, (associated.box.Center().y - (5 * tileSize)) / tileSize};
	front.GetRotated(associated.angleDeg);
	back.GetRotated(associated.angleDeg - PI);

	if (associated.box.x > 1280)
		associated.box.x = 1280;
	else if (associated.box.x < 0)
		associated.box.x = 0;

	if (associated.box.y > 1280)
		associated.box.y = 1280;
	else if (associated.box.y < 0)
		associated.box.y = 0;

	if (hp <= 0)
	{
		associated.RequestDelete();
		Camera::Unfollow();
		std::shared_ptr<GameObject> cannon = pcannon.lock();
		if (cannon)
		{
			cannon->RequestDelete();
		}

		GameObject *go = new GameObject();
		std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
		std::shared_ptr<GameObject> ptr = weak_ptr.lock();

		Sprite *sp = new Sprite(*ptr, "assets/img/penguindeath.png", 5, 0.1, 5 * 0.1);
		Sound *so = new Sound(*ptr, "assets/audio/boom.wav");
		ptr->box.Centered(associated.box.Center());
		ptr->AddComponent(sp);
		ptr->AddComponent(so);

		so->Play(1);
	}
}

void PenguinBody::Render()
{
}

bool PenguinBody::Is(std::string type)
{
	return !strcmp(type.c_str(), "PenguinBody");
}

void PenguinBody::NotifyCollision(GameObject &other)
{
	if (other.GetComponent("Bullet") && static_cast<Bullet *>(other.GetComponent("Bullet"))->targetsPlayer)
	{
		hp -= static_cast<Bullet *>(other.GetComponent("Bullet"))->GetDamage();
	}
	// if (other.GetComponent("Alien") && static_cast<Alien *>(other.GetComponent("Alien"))->GetPos().x)
}

Vec2 PenguinBody::GetPos()
{
	return associated.box.Center();
}