#include "AttackRune.h"
#include "Sprite.h"
#include "Yawara.h"
#include "Timer.h"

/* 1 < factor <= 2 increased attack damage */
AttackRune::AttackRune(GameObject& associated, float attFactor) : Item(associated){

    sp = new Sprite(associated, "assets/img/items/runa.png");
    sp->SetScale(0.5, 0.5);
	associated.AddComponent(sp);

    this->attFactor = attFactor;
}

void AttackRune::Update(float dt){

    static Timer blendTimer;
    static bool active = true;
    
    if(Yawara::player){
        float dist = (associated.box.Center() - Yawara::player->GetPos()).Modulo();

        if(dist <= 45 && active){
            Yawara::player->Boost(Yawara::ATTBOOST, attFactor);
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

void AttackRune::Render(){


}

bool AttackRune::Is(std::string type){

	return !strcmp(type.c_str(), "AttackRune");
}

void AttackRune::Start(){


}