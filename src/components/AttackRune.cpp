#include "AttackRune.h"
#include "Sprite.h"
#include "Yawara.h"
#include "Timer.h"
#include "Easing.h"

#include <algorithm>

#define BASE_ATTRUNE_FILE           "assets/img/items/runa_base.png"
#define BASE_ATTRUNE_FRAMES         3
#define TOP_ATTRUNE_FILE            "assets/img/items/runa_imagem.png"
#define TOP_ATTRUNE_FRAMES          2

#define ATTRUNE_ACTIVATION_DISTANCE 45
#define ATTRUNE_COOLDOWN_TIME       30

/* 1 < factor <= 2 increased attack damage */
AttackRune::AttackRune(GameObject& associated, float attFactor, Color color, Image img) : Item(associated){

    sp = new Sprite(associated, BASE_ATTRUNE_FILE, BASE_ATTRUNE_FRAMES);
    sp->SetScale(1, 1);
    sp->SetFrame(color);
    sp->SetStopFrame(color);
	associated.AddComponent(sp);

    activationSound = new Sound(associated, RUNE_SOUND_PATH);
    associated.AddComponent(activationSound);

    top_layer_sprite = nullptr;

    this->attFactor = attFactor;

    this->top_img = img;
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
            top_layer_sprite = new Sprite(associated, TOP_ATTRUNE_FILE, TOP_ATTRUNE_FRAMES);
            top_layer_sprite->SetScale(1, 1);
            top_layer_sprite->SetFrame(top_img);
            top_layer_sprite->SetStopFrame(top_img);
            associated.AddComponent(top_layer_sprite);
            activationSound->Play(1, MIX_MAX_VOLUME);
        }

        cooldownTimer.Update(dt);
        if(cooldownTimer.Get() >= ATTRUNE_COOLDOWN_TIME && !active){
            active = true;
            associated.RemoveComponent(top_layer_sprite);
            top_layer_sprite = nullptr;
        }

        if(top_layer_sprite){
            static float counter = 0;
            static bool fadingIn = true;

            if(fadingIn){
                counter += dt;
                if(counter >= 1){
                    counter = 1;
                    fadingIn = false;
                }
            } else{
                counter -= dt;
                if(counter <= 0){
                    counter = 0;
                    fadingIn = true;
                }
            }

            float ease = QuadraticEaseInOut(counter);

            top_layer_sprite->SetAlphaMod(int (ease * 255));
        }
    }
}

void AttackRune::Render(){


}

bool AttackRune::Is(std::string type){

	return !std::min(strcmp(type.c_str(), "AttackRune"), strcmp(type.c_str(), "Item"));
}

void AttackRune::Start(){


}

void AttackRune::ChangeColor(Color color){
    Sprite* sp = static_cast<Sprite*> (associated.GetComponent("Sprite"));
    if(sp){
        sp->SetFrame(color);
        sp->SetStopFrame(color);
    }
}