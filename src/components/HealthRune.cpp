#include "HealthRune.h"
#include "Sprite.h"
#include "Yawara.h"

HealthRune::HealthRune(GameObject& associated, float hpFactor) : Item(associated){

    Sprite* sp = new Sprite(associated, "assets/img/items/test.png", 6, 0.2);
	associated.AddComponent(sp);

    this->hpFactor = hpFactor;
}

void HealthRune::Update(float dt){
    
    if(Yawara::player){
        float dist = (associated.box.Center() - Yawara::player->GetPos()).Modulo();

        if(dist <= 45){
            Yawara::player->Boost(Yawara::HPBOOST, hpFactor);
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