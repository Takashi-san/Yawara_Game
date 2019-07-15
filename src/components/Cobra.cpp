#include "Cobra.h"
#include "Sprite.h"
#include "InputManager.h"
#include "Camera.h"
#include "State.h"
#include "Game.h"
#include "Collider.h"
#include "Bullet.h"
#include "Sound.h"
#include "Yawara.h"
#include "Claw.h"
#include "Tongue.h"
#include "Easing.h"

Cobra *Cobra::boss;

#define STATIK_FRAMES 10
#define DEATH_FRAMES 6.0
#define DEATH_FRAME_TIME 0.1

const std::string COBRA_STATIK		= "assets/img/cobra/cobra_parada.png";

// Death sprites

const std::string COBRA_DEATH		= "assets/img/cobra/morte_cobra.png";
const std::string COBRA_EFFECT_1	= "assets/img/capelobo/camada1_efeitomorteboss.png";
const std::string COBRA_EFFECT_2	= "assets/img/capelobo/camada2_efeitomorteboss.png";
const std::string COBRA_EFFECT_3	= "assets/img/capelobo/camada3_efeitomorteboss.png";
const std::string COBRA_EFFECT_4	= "assets/img/capelobo/camada4_efeitomorteboss.png";
const std::string COBRA_EFFECT_5	= "assets/img/capelobo/camada5_efeitomorteboss.png";

// Cobra sounds

const std::string COBRA_HIT_SOUND	= "assets/audio/sons/cobra/hit.ogg";
const std::string COBRA_DEATH_SOUND	= "assets/audio/sons/cobra/death.ogg";

Cobra::Cobra(GameObject &associated) : Enemy(associated)
{
	boss = this;
	timesPlayed = 0;

	hp = 60;
}

void Cobra::Start()
{
	Sprite *sp = new Sprite(associated, COBRA_STATIK, STATIK_FRAMES, 0.100);
	Collider *cl = new Collider(associated);
	associated.AddComponent(sp);
	associated.AddComponent(cl);

	GameObject *go = new GameObject();
	std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
	std::shared_ptr<GameObject> ptr = weak_ptr.lock();
	so = new Sound(*ptr, COBRA_HIT_SOUND);

	// Reset all timers
	restTimer.Restart();
	moveTimer.Restart();
	attackTimer.Restart();
	hitTimer.Restart();
	soundTimer.Restart();
	deathTimer.Restart();
}

Cobra::~Cobra()
{
	// Cobra is dead
	boss = nullptr;
}

void Cobra::Update(float dt)
{
	GameObject *go = new GameObject();
	std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
	std::shared_ptr<GameObject> ptr = weak_ptr.lock();

	hitTimer.Update(dt);
	soundTimer.Update(dt);

	// Cobra is dead
	if (hp <= 0)
	{
		change_sprite = true;
		deathTimer.Update(dt);
		
		Sprite *sp;

		if(deathTimer.Get() < DEATH_EFFECT_TIME){
			if(deathTimer.Get() < DEATH_EFFECT_TIME / 10){
				sp = new Sprite(*ptr, COBRA_EFFECT_1, 1, DEATH_EFFECT_TIME / 10, DEATH_EFFECT_TIME / 10);
				timesPlayed = 0;
			}
			else if (deathTimer.Get() < (DEATH_EFFECT_TIME * 2) / 10)
				sp = new Sprite(*ptr, COBRA_EFFECT_2, 1, DEATH_EFFECT_TIME / 10, DEATH_EFFECT_TIME / 10);
			else if (deathTimer.Get() < (DEATH_EFFECT_TIME * 3) / 10){
				sp = new Sprite(*ptr, COBRA_EFFECT_4, 1, DEATH_EFFECT_TIME / 10, DEATH_EFFECT_TIME / 10);
				sp->SetScale(Vec2{0.5,0.5});
			}
			else if(deathTimer.Get() < (DEATH_EFFECT_TIME * 4) / 10)
				sp = new Sprite(*ptr, COBRA_EFFECT_4, 1, DEATH_EFFECT_TIME / 10, DEATH_EFFECT_TIME / 10);
			else if(deathTimer.Get() < (DEATH_EFFECT_TIME * 5) / 10){
				sp = new Sprite(*ptr, COBRA_EFFECT_4, 1, DEATH_EFFECT_TIME / 10, DEATH_EFFECT_TIME / 10);
				sp->SetColorMod(150,30,140);
			}
			else{
				sp = new Sprite(*ptr, COBRA_EFFECT_5, 7, DEATH_EFFECT_TIME /10, 7 * (DEATH_EFFECT_TIME /10));
				sp->SetColorMod(150,30,140);
				sp->SetAlphaMod(200);
				// sp->SetBlendMode(BLEND_ADD);
				if(timesPlayed == 0){
					++timesPlayed;
				}
			}
		}
		else{
			associated.RequestDelete();

			sp = new Sprite(*ptr, COBRA_DEATH, DEATH_FRAMES, DEATH_FRAME_TIME, DEATH_FRAMES * DEATH_FRAME_TIME);
			so->Open(COBRA_DEATH_SOUND);
			so->Play(1);
		}
		if(ptr)
			ptr->box.Centered(associated.box.Center());
		if(ptr)
			ptr->AddComponent(sp);
	}
}

void Cobra::Render()
{
}

bool Cobra::Is(std::string type)
{
	return !std::min(strcmp(type.c_str(), "Cobra"),strcmp(type.c_str(), "Enemy"));
}

void Cobra::HitSound(){
    so->Open(COBRA_HIT_SOUND);
    so->Play(1,MIX_MAX_VOLUME);
}