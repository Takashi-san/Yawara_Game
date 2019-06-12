#include "Yawara.h"

#include "Game.h"
#include "Sprite.h"
#include "Collider.h"
#include "InputManager.h"
#include "Camera.h"
#include "Claw.h"
#include "Sound.h"
#include "Tapu.h"
#include "Floor.h"

Yawara *Yawara::player;

Yawara::Yawara(GameObject &associated) : Component(associated)
{
	player = this;

	Sprite *sp = new Sprite(associated, "assets/img/yawara/yawara_walk_right.png", 12, 0.100);
	associated.AddComponent(sp);
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);

	sp->SetScale({0.7, 0.7});
	cl->SetScale({0.7, 0.7});

	hp = 100;
	speed = {0, 0};
	dir = RIGHT;
	idle = true;
}

Yawara::~Yawara()
{
	player = nullptr;
}

void Yawara::Start()
{

	GameObject *go = new GameObject();
	std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
	std::shared_ptr<GameObject> ptr = weak_ptr.lock();
	tapu = weak_ptr;

	Tapu *tp = new Tapu(*ptr, Game::GetInstance().GetCurrentState().GetObjectPtr(&associated));
	ptr->box.x = associated.box.x;
	ptr->box.y = associated.box.y;
	ptr->AddComponent(tp);
}

void Yawara::Update(float dt)
{
	InputManager &input = InputManager::GetInstance();

	bool moveAllowed = 1;

	if (input.IsKeyDown(W_KEY))
	{
		safeY = associated.box.Center().y - SAFE_DISTANCE_UP;
		if (input.IsKeyDown(A_KEY))
		{
			safeX = associated.box.Center().x - SAFE_DISTANCE_DOWN;

			speed = {-YAWARA_SPEED / 2, -YAWARA_SPEED / 2};
			if (dir != LEFT_UP)
			{
				change_sprite = true;
			}
			dir = LEFT_UP;

			moveAllowed = (Floor::loaded && Floor::AtAllowedArea(safeX, safeY, 0));
		}
		else if (input.IsKeyDown(D_KEY))
		{
			safeX = associated.box.Center().x + SAFE_DISTANCE_DOWN;

			speed = {YAWARA_SPEED / 2, -YAWARA_SPEED / 2};
			if (dir != RIGHT_UP)
			{
				change_sprite = true;
			}
			dir = RIGHT_UP;

			moveAllowed = (Floor::loaded && Floor::AtAllowedArea(safeX, safeY, 0));
		}
		else
		{
			safeX = associated.box.Center().x;

			speed = {0, -YAWARA_SPEED};
			if (dir != UP)
			{
				change_sprite = true;
			}
			dir = UP;

			moveAllowed = (Floor::loaded && Floor::AtAllowedArea(safeX, safeY, 0));
		}

		if (idle)
		{
			change_sprite = true;
		}
		idle = false;
	}
	else if (input.IsKeyDown(S_KEY))
	{
		safeY = associated.box.Center().y + SAFE_DISTANCE_UP;
		if (input.IsKeyDown(A_KEY))
		{
			safeX = associated.box.Center().x - SAFE_DISTANCE_DOWN;

			speed = {-YAWARA_SPEED / 2, YAWARA_SPEED / 2};
			if (dir != LEFT_DOWN)
			{
				change_sprite = true;
			}
			dir = LEFT_DOWN;

			moveAllowed = (Floor::loaded && Floor::AtAllowedArea(safeX, safeY, 0));
		}
		else if (input.IsKeyDown(D_KEY))
		{
			safeX = associated.box.Center().x + SAFE_DISTANCE_DOWN;

			speed = {YAWARA_SPEED / 2, YAWARA_SPEED / 2};
			if (dir != RIGHT_DOWN)
			{
				change_sprite = true;
			}
			dir = RIGHT_DOWN;

			moveAllowed = (Floor::loaded && Floor::AtAllowedArea(safeX, safeY, 0));
		}
		else
		{
			safeX = associated.box.Center().x;

			speed = {0, YAWARA_SPEED};
			if (dir != DOWN)
			{
				change_sprite = true;
			}
			dir = DOWN;
			moveAllowed = (Floor::loaded && Floor::AtAllowedArea(safeX, safeY, 0));
		}

		if (idle)
		{
			change_sprite = true;
		}
		idle = false;
	}
	else if (input.IsKeyDown(A_KEY))
	{
		safeX = associated.box.Center().x - SAFE_DISTANCE_DOWN;
		safeY = associated.box.Center().y;

		speed = {-YAWARA_SPEED, 0};
		if (dir != LEFT)
		{
			change_sprite = true;
		}
		dir = LEFT;

		moveAllowed = (Floor::loaded && Floor::AtAllowedArea(safeX, safeY, 0));

		if (idle)
		{
			change_sprite = true;
		}
		idle = false;
	}
	else if (input.IsKeyDown(D_KEY))
	{
		safeX = associated.box.Center().x + SAFE_DISTANCE_DOWN;
		safeY = associated.box.Center().y;

		speed = {YAWARA_SPEED, 0};
		if (dir != RIGHT)
		{
			change_sprite = true;
		}
		dir = RIGHT;

		moveAllowed = (Floor::loaded && Floor::AtAllowedArea(safeX, safeY, 0));

		if (idle)
		{
			change_sprite = true;
		}
		idle = false;
	}
	else
	{
		speed = {0, 0};

		if (!idle)
		{
			change_sprite = true;
		}
		idle = true;
	}

	if (change_sprite)
	{
		Vec2 position = associated.box.Center();
		change_sprite = false;
		if (idle)
		{
			Sprite *sp = static_cast<Sprite *>(associated.GetComponent("Sprite"));
			if (sp)
			{
				switch (dir)
				{
				case RIGHT:
					// sp->Open("assets/img/yawara/yawara_idle_right.png");
					// sp->SetFrameCount(12);
					break;

				case LEFT:
					// sp->Open("assets/img/yawara/yawara_idle_left.png");
					// sp->SetFrameCount(12);
					break;

				case UP:
					sp->Open("assets/img/yawara/yawara_idle_up.png");
					sp->SetFrameCount(1);
					break;

				case DOWN:
					sp->Open("assets/img/yawara/yawara_idle_down.png");
					sp->SetFrameCount(1);
					break;

				case RIGHT_UP:
					sp->Open("assets/img/yawara/yawara_idle_up_right.png");
					sp->SetFrameCount(1);
					break;

				case RIGHT_DOWN:
					sp->Open("assets/img/yawara/yawara_idle_down_right.png");
					sp->SetFrameCount(1);
					break;

				case LEFT_UP:
					sp->Open("assets/img/yawara/yawara_idle_up_left.png");
					sp->SetFrameCount(1);
					break;

				case LEFT_DOWN:
					sp->Open("assets/img/yawara/yawara_idle_down_left.png");
					sp->SetFrameCount(1);
					break;

				default:
					break;
				}
			}
		}
		else
		{
			Sprite *sp = static_cast<Sprite *>(associated.GetComponent("Sprite"));
			if (sp)
			{
				switch (dir)
				{
				case RIGHT:
					sp->Open("assets/img/yawara/yawara_walk_right.png");
					sp->SetFrameCount(12);
					break;

				case LEFT:
					sp->Open("assets/img/yawara/yawara_walk_left.png");
					sp->SetFrameCount(12);
					break;

				case UP:
					sp->Open("assets/img/yawara/yawara_run_up.png");
					sp->SetFrameCount(9);
					break;

				case DOWN:
					sp->Open("assets/img/yawara/yawara_run_down.png");
					sp->SetFrameCount(9);
					break;

				case RIGHT_UP:
					// sp->Open("assets/img/yawara/yawara_run_up_right.png");
					// sp->SetFrameCount(1);
					break;

				case RIGHT_DOWN:
					// sp->Open("assets/img/yawara/yawara_run_down_right.png");
					// sp->SetFrameCount(1);
					break;

				case LEFT_UP:
					sp->Open("assets/img/yawara/yawara_run_up_left.png");
					sp->SetFrameCount(9);
					break;

				case LEFT_DOWN:
					sp->Open("assets/img/yawara/yawara_run_down_left.png");
					sp->SetFrameCount(9);
					break;

				default:
					break;
				}
			}
		}
		associated.box.Centered(position);
	}

	if (moveAllowed)
	{
		associated.box.x += speed.x * dt;
		associated.box.y += speed.y * dt;
	}

	if (hp <= 0)
	{
		associated.RequestDelete();
		Camera::Unfollow();

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

void Yawara::Render()
{
}

bool Yawara::Is(std::string type)
{
	return !strcmp(type.c_str(), "Yawara");
}

void Yawara::NotifyCollision(GameObject &other)
{
	Claw *claw = static_cast<Claw *>(other.GetComponent("Claw"));

	if (claw && claw->targetsPlayer)
	{
		// hp -= claw->GetDamage();
	}
}

Vec2 Yawara::GetPos()
{
	return associated.box.Center();
}