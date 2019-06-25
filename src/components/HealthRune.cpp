#include "HealthRune.h"
#include "Sprite.h"
#include "Yawara.h"
#include "Timer.h"

#define BASE_HEALTHRUNE_FILE            "assets/img/items/cristal_base.png"
#define BASE_HEALTHRUNE_FRAMES          3
#define TOP_HEALTHRUNE_FILE             "assets/img/items/cristal_brilho.png"

#define HEALTHRUNE_ACTIVATION_DISTANCE  45
#define HEALTHRUNE_COOLDOWN_TIME        30

/* 1 < factor <= 2 means an increased health */
HealthRune::HealthRune(GameObject& associated, float hpFactor) : Item(associated){

    sp = new Sprite(associated, BASE_HEALTHRUNE_FILE, BASE_HEALTHRUNE_FRAMES, 0.5);
    sp->SetScale(0.5, 0.5);
    sp->SetFrame(2);
    sp->SetStopFrame(2);
	associated.AddComponent(sp);

    this->hpFactor = hpFactor;
}

void HealthRune::Update(float dt){

    static Timer cooldownTimer;
    static bool active = true;
    
    if(Yawara::player){
        float dist = (associated.box.Center() - Yawara::player->GetPos()).Modulo();

        if(dist <= HEALTHRUNE_ACTIVATION_DISTANCE && active){
            Yawara::player->Boost(Yawara::HPBOOST, hpFactor);
            cooldownTimer.Restart();
            active = false;
            top_layer_sprite = new Sprite(associated, TOP_HEALTHRUNE_FILE);
            top_layer_sprite->SetScale(0.5, 0.5);
            associated.AddComponent(top_layer_sprite);
        }

        cooldownTimer.Update(dt);
        if(cooldownTimer.Get() >= HEALTHRUNE_COOLDOWN_TIME && !active){
            sp->SetBlendMode(BLEND_BLEND);
            active = true;
            associated.RemoveComponent(top_layer_sprite);
        }
    }
}

void HealthRune::Render(){


}

bool HealthRune::Is(std::string type){

	return !strcmp(type.c_str(), "HealthRune");
}

void HealthRune::Start(){


}