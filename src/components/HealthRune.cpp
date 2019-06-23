#include "HealthRune.h"
#include "Sprite.h"
#include "Yawara.h"
#include "Timer.h"

/* 1 < factor <= 2 means an increased health */
HealthRune::HealthRune(GameObject& associated, float hpFactor) : Item(associated){

    sp = new Sprite(associated, "assets/img/items/cristal.png");
    sp->SetScale(0.5, 0.5);
	associated.AddComponent(sp);

    this->hpFactor = hpFactor;
}

void HealthRune::Update(float dt){

    static Timer blendTimer;
    static bool active = true;
    
    if(Yawara::player){
        float dist = (associated.box.Center() - Yawara::player->GetPos()).Modulo();

        if(dist <= 45 && active){
            Yawara::player->Boost(Yawara::HPBOOST, hpFactor);
            sp->SetBlendMode(BLEND_ADD);
            blendTimer.Restart();
            active = false;
        }

        blendTimer.Update(dt);
        if(blendTimer.Get() >= 30){
            sp->SetBlendMode(BLEND_BLEND);
            active = true;
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