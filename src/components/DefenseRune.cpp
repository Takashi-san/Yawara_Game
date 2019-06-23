#include "DefenseRune.h"
#include "Sprite.h"
#include "Yawara.h"
#include "Timer.h"

/* 1 < factor <= 2 means an increased defense */
DefenseRune::DefenseRune(GameObject& associated, float defFactor) : Item(associated){

    sp = new Sprite(associated, "assets/img/items/corrupcao.png");
    sp->SetScale(0.5, 0.5);
	associated.AddComponent(sp);

    this->defFactor = defFactor;
}

void DefenseRune::Update(float dt){

    static Timer blendTimer;
    static bool active = true;
    
    if(Yawara::player){
        float dist = (associated.box.Center() - Yawara::player->GetPos()).Modulo();

        if(dist <= 45 && active){
            Yawara::player->Boost(Yawara::DEFBOOST, defFactor);
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

void DefenseRune::Render(){


}

bool DefenseRune::Is(std::string type){

	return !strcmp(type.c_str(), "DefenseRune");
}

void DefenseRune::Start(){


}