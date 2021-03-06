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
#define TAPU_BULLET_DAMAGE	15
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

#define TAPU_BULLET			"assets/img/tapu/disparo.png"
#define TAPU_BULLET_END		"assets/img/tapu/disparo_colisão.png"
#define TAPU_BULLET_FRAME	5
#define TAPU_BULLET_TIME	0.05

#define TAPU_SHOOT			"assets/img/tapu/efeito_disparo.png"
#define TAPU_SHOOT_FRAME	5
#define TAPU_SHOOT_TIME		0.05
#define TAPU_SHOOT_SOUND	"assets/audio/sons/tapu/disparo.ogg"

Tapu::Tapu(GameObject& associated, std::weak_ptr<GameObject> Yawara) : Component(associated) {
	shoot = new Sound(associated, TAPU_SHOOT_SOUND);

	angle = 0;
	this->yawara = Yawara;
	dir = RIGHT;
	changedDir = false;
	damageFactor = 1;
}

void Tapu::Start() {
	Sprite* sp = new Sprite(associated, TAPU_R, TAPU_FRAMES, TAPU_ANI_TIME);
	associated.AddComponent(sp);
	Collider *cl = new Collider(associated);
	associated.AddComponent(cl);
	height = sp->GetHeight();

	GameObject* go = new GameObject();
	shadow_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
	std::shared_ptr<GameObject> ptr = shadow_ptr.lock();
	Sprite* shadow = new Sprite(*ptr, "assets/img/tapu/shadow.png");
	ptr->box.Centered(associated.box.Center());
	ptr->AddComponent(shadow);
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

		if(dist.Modulo() > TAPU_RAIO){
			newPos = YwrCenter + ((dist/dist.Modulo()) * TAPU_RAIO);
			angle = atan2(input.GetMouseY() + Camera::pos.y - TapuCenter.y, input.GetMouseX() + Camera::pos.x - TapuCenter.x);
		}
		associated.box.Centered(newPos);

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

		float ease = SineEaseInOut(counter);

		associated.box.y += ease * 10;

		std::shared_ptr<GameObject> shadow = shadow_ptr.lock();
		if(shadow){
			shadow->box.Centered(associated.box.Center().x, (height) + associated.box.Center().y - (ease * 10));
			Sprite* shadow_sprite = static_cast<Sprite*> (shadow->GetComponent("Sprite"));
			if(shadow_sprite){
				shadow_sprite->SetScale(1 + (0.45 * ease), 1 + (0.45 * ease));
			}
		}

		std::shared_ptr<GameObject> muzzle = shoot_fx.lock();
		if(muzzle){
			muzzle->box.Centered(associated.box.Center());
			muzzle->box.x += shoot_fx_offset.x;
			muzzle->box.y += shoot_fx_offset.y;
		}		

		dist = TapuCenter - YwrCenter;
		changedDir = false;

		float degAngle = angle * 180/M_PI;

		if(degAngle >= -22.5 && degAngle <= 22.5){
			angle = 0;
			if(dir != RIGHT){
				dir = RIGHT;
				changedDir = true;
			}
		} else if(degAngle < -22.5 && degAngle >= -67.5){
			angle = -M_PI/4;
			if (dir != UP_RIGHT){
				dir = UP_RIGHT;
				changedDir = true;
			}
		} else if(degAngle < -67.5 && degAngle >= -112.5){
			angle = -M_PI/2;
			if (dir != UP){
				dir = UP;
				changedDir = true;
			}
		} else if(degAngle < -112.5 && degAngle >= -157.5){
			angle = -3*M_PI/4;
			if (dir != UP_LEFT){
				dir = UP_LEFT;
				changedDir = true;
			}
		} else if((degAngle < -112.5 && degAngle >= -180) || (degAngle > 157.5 && degAngle <= 180)){
			angle = M_PI;
			if (dir != LEFT){
				dir = LEFT;
				changedDir = true;
			}
		} else if(degAngle <= 157.5 && degAngle > 112.5){
			angle = 3*M_PI/4;
			if (dir != DOWN_LEFT){
				dir = DOWN_LEFT;
				changedDir = true;
			}
		} else if(degAngle <= 112.5 && degAngle > 67.5){
			angle = M_PI/2;
			if (dir != DOWN){
				dir = DOWN;
				changedDir = true;
			}
		} else if(degAngle <= 67.5 && degAngle > 22.5){
			angle = M_PI/4;
			if (dir != DOWN_RIGHT){
				dir = DOWN_RIGHT;
				changedDir = true;
			}
		}

		cd.Update(dt);

		if (input.MousePress(LEFT_MOUSE_BUTTON) && cd.Get() > TAPU_SHOOT_CD)
		{
			Shoot();
			cd.Restart();
		}

	} else {
		associated.RequestDelete();
		std::shared_ptr<GameObject> shadow = shadow_ptr.lock();
		if(shadow){
			shadow->RequestDelete();
		}
	}
}

void Tapu::Render()
{
	Sprite *sp = static_cast<Sprite *>(associated.GetComponent("Sprite"));
	if (sp && changedDir)
	{
	int currFrame = sp->GetFrame();

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

		sp->SetFrame(currFrame);
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
	std::weak_ptr<GameObject> weak_bullet = Game::GetInstance().GetCurrentState().AddObject(go);
	std::shared_ptr<GameObject> ptr_bullet = weak_bullet.lock();
	
	Bullet *bam = new Bullet(*ptr_bullet, angle, TAPU_BULLET_SPEED, TAPU_BULLET_DAMAGE * damageFactor, TAPU_BULLET_RANGE, TAPU_BULLET, TAPU_BULLET_END, TAPU_BULLET_FRAME, TAPU_BULLET_TIME, false);
	ptr_bullet->box.Centered(associated.box.Center());
	Vec2 offset(associated.box.w / 2, 0);
	offset.Rotate(angle);
	ptr_bullet->box.x += offset.x;
	ptr_bullet->box.y += offset.y;
	ptr_bullet->AddComponent(bam);

	go = new GameObject();
	shoot_fx = Game::GetInstance().GetCurrentState().AddObject(go);
	ptr = shoot_fx.lock();

	Sprite* sp = new Sprite(*ptr, TAPU_SHOOT, TAPU_SHOOT_FRAME, TAPU_SHOOT_TIME, TAPU_SHOOT_FRAME * TAPU_SHOOT_TIME);
	sp->SetFrame(0);
	ptr->AddComponent(sp);	
	ptr->box.Centered(associated.box.Center());
	ptr->box.x += offset.x;
	ptr->box.y += offset.y;
	shoot_fx_offset = offset;
	ptr->angleDeg = angle/0.0174533;

	shoot->PlayFadeIn(800);
}

void Tapu::SetDamageFactor(float factor) {
	damageFactor = factor;
}

float Tapu::GetDamageFactor() {

	return damageFactor;
}
		
void Tapu::NotifyCollision(GameObject &other)
{
	if (other.GetComponent("Bullet") && static_cast<Bullet *>(other.GetComponent("Bullet"))->targetsPlayer)
	{
	}
}