#include "Tapu.h"
#include "InputManager.h"
#include "Game.h"
#include "Bullet.h"
#include "Camera.h"
#include "Sprite.h"
#include "Collider.h"
#include "Timer.h"

#define TAPU_BULLET_SPEED	750
#define TAPU_BULLET_DAMAGE	10
#define TAPU_BULLET_RANGE	1000
#define TAPU_SHOOT_CD		0.4
#define TAPU_RAIO			100

#define TAPU_ANI_TIME	0.12
#define TAPU_R			"assets/img/tapu/tapu_right.png"
#define TAPU_R_FRAME	9
#define TAPU_L			"assets/img/tapu/tapu_left.png"
#define TAPU_L_FRAME	9

#define TAPU_BULLET			"assets/penguin/img/minionbullet2.png"
#define TAPU_BULLET_FRAME	3
#define TAPU_BULLET_TIME	0.04

Tapu::Tapu(GameObject& associated, std::weak_ptr<GameObject> Yawara) : Component(associated) {

	Sprite* sp = new Sprite(associated, TAPU_R, TAPU_R_FRAME, TAPU_ANI_TIME);
	associated.AddComponent(sp);
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);

	angle = 0;
	this->yawara = Yawara;
    radius = TAPU_RAIO;
	dir = LEFT;
	changedDir = false;
}

void Tapu::Update(float dt)
{
	static Timer cd;
	InputManager &input = InputManager::GetInstance();

	std::shared_ptr<GameObject> ywr = yawara.lock();
	if (ywr)
	{

		Vec2 newPos(input.GetMouseX() + Camera::pos.x, input.GetMouseY() + Camera::pos.y);
		Vec2 dist = newPos - ywr->box.Center();

		angle = 0;
		associated.angleDeg = 0;

		if (dist.Modulo() > radius)
		{
			newPos = ywr->box.Center() + ((dist / dist.Modulo()) * radius);
			angle = atan2(input.GetMouseY() + Camera::pos.y - associated.box.Center().y, input.GetMouseX() + Camera::pos.x - associated.box.Center().x);
			associated.angleDeg = angle / 0.0174533;
		}
		associated.box.Centered(newPos);
		cd.Update(dt);

		dist = associated.box.Center() - ywr->box.Center();
		changedDir = false;

		if (dist.x >= 0)
		{
			if (dir == LEFT)
			{
				changedDir = true;
				dir = RIGHT;
			}
		}
		else
		{
			if (dir == RIGHT)
			{
				dir = LEFT;
				changedDir = true;
			}
		}

		if (dir == LEFT && (dist.Modulo() >= (radius - 1)))
		{
			associated.angleDeg += 180;
		}
		else if (dir == LEFT)
		{
			angle += M_PI;
		}

		if (input.MousePress(LEFT_MOUSE_BUTTON) && cd.Get() > TAPU_SHOOT_CD)
		{
			Shoot();
			cd.Restart();
		}

		if (associated.angleDeg >= 360)
		{
			associated.angleDeg -= 360;
		}
		if (associated.angleDeg <= -360)
		{
			associated.angleDeg += 360;
		}
	}
	else
	{
		associated.RequestDelete();
	}
}

void Tapu::Render()
{

	Sprite *sp = static_cast<Sprite *>(associated.GetComponent("Sprite"));
	if (sp && changedDir)
	{

		switch (dir)
		{
			case LEFT:
				sp->Open(TAPU_L);
				break;
			
			case RIGHT:
				sp->Open(TAPU_R);
				break;
		}		
	}
}

bool Tapu::Is(std::string type)
{
	return !strcmp(type.c_str(), "Tapu");
}

void Tapu::Shoot()
{
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;

	GameObject *go = new GameObject();
	weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
	ptr = weak_ptr.lock();
	
	Bullet *bam = new Bullet(*ptr, angle, TAPU_BULLET_SPEED, TAPU_BULLET_DAMAGE, TAPU_BULLET_RANGE, TAPU_BULLET, TAPU_BULLET_FRAME, TAPU_BULLET_TIME, false);
	ptr->box.Centered(associated.box.Center());
	Vec2 offset(associated.box.w / 2, 0);
	offset.Rotate(angle);
	ptr->box.x += offset.x;
	ptr->box.y += offset.y;
	ptr->AddComponent(bam);
}

void Tapu::NotifyCollision(GameObject &other)
{
	if (other.GetComponent("Bullet") && static_cast<Bullet *>(other.GetComponent("Bullet"))->targetsPlayer)
	{
	}
}