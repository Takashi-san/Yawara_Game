#include "Tapu.h"
#include "InputManager.h"
#include "Game.h"
#include "Bullet.h"
#include "Camera.h"
#include "Sprite.h"
#include "Collider.h"
#include "Timer.h"
#include "Easing.h"

#define TAPU_BULLET_SPEED	750
#define TAPU_BULLET_DAMAGE	10
#define TAPU_BULLET_RANGE	1000
#define TAPU_SHOOT_CD		0.4
#define TAPU_RAIO			100

#define TAPU_ANI_TIME	0.14
#define TAPU_FRAMES		9
#define TAPU_R			"assets/img/tapu/tapu_right.png"
#define TAPU_L			"assets/img/tapu/tapu_left.png"
#define TAPU_U			"assets/img/tapu/tapu_up.png"
#define TAPU_D			"assets/img/tapu/tapu_down.png"
#define TAPU_UR			"assets/img/tapu/tapu_up_right.png"
#define TAPU_UL			"assets/img/tapu/tapu_up_left.png"
#define TAPU_DR			"assets/img/tapu/tapu_down_right.png"
#define TAPU_DL			"assets/img/tapu/tapu_down_left.png"

#define TAPU_BULLET			"assets/penguin/img/minionbullet2.png"
#define TAPU_BULLET_FRAME	3
#define TAPU_BULLET_TIME	0.04

Tapu::Tapu(GameObject& associated, std::weak_ptr<GameObject> Yawara) : Component(associated) {

	Sprite* sp = new Sprite(associated, TAPU_R, TAPU_FRAMES, TAPU_ANI_TIME);
	associated.AddComponent(sp);
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);

	angle = 0;
	this->yawara = Yawara;
    radius = TAPU_RAIO;
	dir = RIGHT;
	changedDir = false;
}

void Tapu::Update(float dt)
{
	static Timer cd;
	static float counter = 0;
	static bool goingUp = true;
	InputManager& input = InputManager::GetInstance();

	std::shared_ptr<GameObject> ywr = yawara.lock();
	if (ywr)
	{

		Vec2 newPos(input.GetMouseX() + Camera::pos.x, input.GetMouseY() + Camera::pos.y);
		Vec2 TapuCenter(associated.box.Center()), YwrCenter(ywr->box.Center());
		Vec2 dist = newPos - YwrCenter;

		angle = atan2(TapuCenter.y - YwrCenter.y, TapuCenter.x - YwrCenter.x);

		if(dist.Modulo() > radius){
			newPos = YwrCenter + ((dist/dist.Modulo()) * radius);
			angle = atan2(input.GetMouseY() + Camera::pos.y - TapuCenter.y, input.GetMouseX() + Camera::pos.x - TapuCenter.x);
		}
		associated.box.Centered(newPos);

		// associated.angleDeg = angle*180/M_PI;

		if(goingUp){
			counter += TAPU_ANI_TIME*TAPU_FRAMES*dt;
			if(counter >= 1){
				counter = 1;
				goingUp = false;
			}
		} else{
			counter -= TAPU_ANI_TIME*TAPU_FRAMES*dt;
			if(counter <= 0){
				counter = 0;
				goingUp = true;
			}
		}

		associated.box.y += SineEaseInOut(counter) * 10;

		dist = TapuCenter - YwrCenter;
		changedDir = false;

		float degAngle = angle * 180/M_PI;

		if(degAngle >= -22.5 && degAngle <= 22.5){
			if(dir != RIGHT){
				dir = RIGHT;
				changedDir = true;
			}
		} else if(degAngle < -22.5 && degAngle >= -67.5){
			if (dir != UP_RIGHT){
				dir = UP_RIGHT;
				changedDir = true;
			}
		} else if(degAngle < -67.5 && degAngle >= -112.5){
			if (dir != UP){
				dir = UP;
				changedDir = true;
			}
		} else if(degAngle < -112.5 && degAngle >= -157.5){
			if (dir != UP_LEFT){
				dir = UP_LEFT;
				changedDir = true;
			}
		} else if((degAngle < -112.5 && degAngle >= -180) || (degAngle > 157.5 && degAngle <= 180)){
			if (dir != LEFT){
				dir = LEFT;
				changedDir = true;
			}
		} else if(degAngle <= 157.5 && degAngle > 112.5){
			if (dir != DOWN_LEFT){
				dir = DOWN_LEFT;
				changedDir = true;
			}
		} else if(degAngle <= 112.5 && degAngle > 67.5){
			if (dir != DOWN){
				dir = DOWN;
				changedDir = true;
			}
		} else if(degAngle <= 67.5 && degAngle > 22.5){
			if (dir != DOWN_RIGHT){
				dir = DOWN_RIGHT;
				changedDir = true;
			}
		}

		switch (dir){
			case LEFT:
				angle = M_PI;
				break;
			
			case RIGHT:
				angle = 0;
				break;
			
			case UP:
				angle = -M_PI/2;
				break;
			
			case DOWN:
				angle = M_PI/2;
				break;
			
			case UP_LEFT:
				angle = -3*M_PI/4;
				break;
			
			case UP_RIGHT:
				angle = -M_PI/4;
				break;
			
			case DOWN_LEFT:
				angle = 3*M_PI/4;
				break;
			
			case DOWN_RIGHT:
				angle = M_PI/4;
				break;
		}

		cd.Update(dt);

		if(input.MousePress(LEFT_MOUSE_BUTTON) && cd.Get() > TAPU_SHOOT_CD){
			Shoot();
			cd.Restart();
		}

	} else {
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
			
			case UP:
				sp->Open(TAPU_U);
				break;
			
			case DOWN:
				sp->Open(TAPU_D);
				break;
			
			case UP_LEFT:
				sp->Open(TAPU_UL);
				break;
			
			case UP_RIGHT:
				sp->Open(TAPU_UR);
				break;
			
			case DOWN_LEFT:
				sp->Open(TAPU_DL);
				break;
			
			case DOWN_RIGHT:
				sp->Open(TAPU_DR);
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