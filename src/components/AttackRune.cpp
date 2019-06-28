#include "AttackRune.h"
#include "Sprite.h"
#include "Yawara.h"
#include "Timer.h"

#define BASE_ATTRUNE_FILE           "assets/img/items/runa_base.png"
#define BASE_ATTRUNE_FRAMES         3
#define TOP_ATTRUNE_FILE            "assets/img/items/runa_imagem.png"
#define TOP_ATTRUNE_FRAMES          2

#define ATTRUNE_ACTIVATION_DISTANCE 45
#define ATTRUNE_COOLDOWN_TIME       30

/* 1 < factor <= 2 increased attack damage */
AttackRune::AttackRune(GameObject& associated, float attFactor) : Item(associated){

    sp = new Sprite(associated, BASE_ATTRUNE_FILE, BASE_ATTRUNE_FRAMES);
    sp->SetScale(1, 1);
    sp->SetFrame(1);
    sp->SetStopFrame(1);
	associated.AddComponent(sp);

    top_layer_sprite = new Sprite(associated, TOP_ATTRUNE_FILE, TOP_ATTRUNE_FRAMES);
    top_layer_sprite->SetScale(1, 1);
    top_layer_sprite->SetFrame(0);
    top_layer_sprite->SetStopFrame(0);
    associated.AddComponent(top_layer_sprite);

    this->attFactor = attFactor;
}

void AttackRune::Update(float dt){

    static Timer cooldownTimer;
    static bool active = true;
    
    if(Yawara::player){
        float dist = (associated.box.Center() - Yawara::player->GetCenterPos()).Modulo();

        if(dist <= ATTRUNE_ACTIVATION_DISTANCE && active){
            Yawara::player->Boost(Yawara::ATTBOOST, attFactor);
            cooldownTimer.Restart();
            active = false;
            associated.RemoveComponent(top_layer_sprite);
        }

        cooldownTimer.Update(dt);
        if(cooldownTimer.Get() >= ATTRUNE_COOLDOWN_TIME && !active){
            active = true;
            top_layer_sprite = new Sprite(associated, TOP_ATTRUNE_FILE, TOP_ATTRUNE_FRAMES);
            top_layer_sprite->SetScale(1, 1);
            top_layer_sprite->SetFrame(0);
            top_layer_sprite->SetStopFrame(0);
            associated.AddComponent(top_layer_sprite);
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