#include "DefenseRune.h"
#include "Sprite.h"
#include "Yawara.h"
#include "Timer.h"

#define BASE_DEFRUNE_FILE               "assets/img/items/espirito_corrompido.png"
#define BASE_DEFRUNE_FRAMES             9
#define BASE_DEFRUNE_FRAMETIME          0.15

#define INTERACTED_DEFRUNE_FILE         "assets/img/items/espirito_corrompido_interacao.png"
#define INTERACTED_DEFRUNE_FRAMES       10
#define INTERACTED_DEFRUNE_FRAMETIME    0.1

#define ACTIVATED_DEFRUNE_FILE          "assets/img/items/espirito.png"
#define ACTIVATED_DEFRUNE_FRAMES        8
#define ACTIVATED_DEFRUNE_FRAMETIME     0.2

#define DEFRUNE_ACTIVATION_DISTANCE     45
#define DEFRUNE_COOLDOWN_TIME           30

/* 1 < factor <= 2 means an increased defense */
DefenseRune::DefenseRune(GameObject& associated, float defFactor) : Item(associated){

    sp = new Sprite(associated, BASE_DEFRUNE_FILE, BASE_DEFRUNE_FRAMES, BASE_DEFRUNE_FRAMETIME);
    sp->SetScale(1, 1);
	associated.AddComponent(sp);

    activationSound = new Sound(associated, RUNE_SOUND_PATH);
    associated.AddComponent(activationSound);

    this->defFactor = defFactor;
}

void DefenseRune::Update(float dt){

    static Timer cooldownTimer, changeTimer;
    static bool active = true, changed = false;
    
    if(Yawara::player){
        float dist = (associated.box.Center() - Yawara::player->GetCenterPos()).Modulo();

        changeTimer.Update(dt);

        if(dist <= DEFRUNE_ACTIVATION_DISTANCE && active){
            Yawara::player->Boost(Yawara::DEFBOOST, defFactor);
            changeTimer.Restart();
            changed = true;
            sp->Open(INTERACTED_DEFRUNE_FILE);
            sp->SetFrameCount(INTERACTED_DEFRUNE_FRAMES);
            sp->SetFrameTime(INTERACTED_DEFRUNE_FRAMETIME);
            sp->SetScale(1, 1);
            associated.box.x -= 16;
            associated.box.y -= 20;
            cooldownTimer.Restart();
            active = false;
            activationSound->Play(1, MIX_MAX_VOLUME);
        }

        if((changeTimer.Get() >= INTERACTED_DEFRUNE_FRAMES * INTERACTED_DEFRUNE_FRAMETIME) && changed && !active){
            sp->Open(ACTIVATED_DEFRUNE_FILE);
            sp->SetFrameCount(ACTIVATED_DEFRUNE_FRAMES);
            sp->SetFrameTime(ACTIVATED_DEFRUNE_FRAMETIME);
            sp->SetScale(1, 1);
            associated.box.x += 48;
            associated.box.y += 40;
            changed = false;
            changeTimer.Restart();
        }

        cooldownTimer.Update(dt);
        if(cooldownTimer.Get() >= DEFRUNE_COOLDOWN_TIME && !active){
            sp->Open(BASE_DEFRUNE_FILE);
            sp->SetFrameCount(BASE_DEFRUNE_FRAMES);
            sp->SetFrameTime(BASE_DEFRUNE_FRAMETIME);
            sp->SetScale(1, 1);
            associated.box.x -= 32;
            associated.box.y -= 20;
            active = true;
            changeTimer.Restart();
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