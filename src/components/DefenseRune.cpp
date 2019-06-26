#include "DefenseRune.h"
#include "Sprite.h"
#include "Yawara.h"
#include "Timer.h"

#define BASE_DEFRUNE_FILE               "assets/img/items/espirito_corrompido.png"
#define BASE_DEFRUNE_FRAMES             4
#define BASE_DEFRUNE_FRAMETIME          0.3
#define ACTIVATED_DEFRUNE_FILE          "assets/img/items/espirito.png"
#define ACTIVATED_DEFRUNE_FRAMES        8
#define ACTIVATED_DEFRUNE_FRAMETIME     0.2

#define DEFRUNE_ACTIVATION_DISTANCE     45
#define DEFRUNE_COOLDOWN_TIME           30

/* 1 < factor <= 2 means an increased defense */
DefenseRune::DefenseRune(GameObject& associated, float defFactor) : Item(associated){

    sp = new Sprite(associated, BASE_DEFRUNE_FILE, BASE_DEFRUNE_FRAMES, BASE_DEFRUNE_FRAMETIME);
    sp->SetScale(0.5, 0.5);
	associated.AddComponent(sp);

    this->defFactor = defFactor;
}

void DefenseRune::Update(float dt){

    static Timer cooldownTimer;
    static bool active = true;
    
    if(Yawara::player){
        float dist = (associated.box.Center() - Yawara::player->GetCenterPos()).Modulo();

        if(dist <= DEFRUNE_ACTIVATION_DISTANCE && active){
            Yawara::player->Boost(Yawara::DEFBOOST, defFactor);
            sp->Open(ACTIVATED_DEFRUNE_FILE);
            sp->SetFrameCount(ACTIVATED_DEFRUNE_FRAMES);
            sp->SetFrameTime(ACTIVATED_DEFRUNE_FRAMETIME);
            sp->SetScale(1, 1);
            cooldownTimer.Restart();
            active = false;
        }

        cooldownTimer.Update(dt);
        if(cooldownTimer.Get() >= DEFRUNE_COOLDOWN_TIME && !active){
            sp->Open(BASE_DEFRUNE_FILE);
            sp->SetFrameCount(BASE_DEFRUNE_FRAMES);
            sp->SetFrameTime(BASE_DEFRUNE_FRAMETIME);
            sp->SetScale(0.5, 0.5);
            active = true;
        }
    }
}

void DefenseRune::Render(){


}

bool DefenseRune::Is(std::string type){

	return !strcmp(type.c_str(), "DefenseRune");
}

void DefenseRune::Start(){


}