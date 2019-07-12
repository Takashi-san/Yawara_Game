#include "HealthRune.h"
#include "Sprite.h"
#include "Yawara.h"
#include "Timer.h"
#include "Easing.h"
#include "InputManager.h"

#include <algorithm>

#define BASE_HEALTHRUNE_FILE            "assets/img/items/cristal_base.png"
#define BASE_HEALTHRUNE_FRAMES          3
#define TOP_HEALTHRUNE_FILE             "assets/img/items/cristal_brilho.png"
#define RESSONANCE_SPRITE               "assets/img/npc/efeito_npc.png"
#define RESSONANCE_FRAME                3
#define RESSONANCE_FRAMETIME            0.25

#define HEALTHRUNE_SOUND                "assets/audio/sons/interacao_cristal.ogg"

#define HEALTHRUNE_ACTIVATION_DISTANCE  800
#define HEALTHRUNE_COOLDOWN_TIME        30

/* 1 < factor <= 2 means an increased health */
HealthRune::HealthRune(GameObject& associated, float hpFactor, Color color) : Item(associated){

    sp = new Sprite(associated, BASE_HEALTHRUNE_FILE, BASE_HEALTHRUNE_FRAMES, 0.5);
    sp->SetScale(1, 1);
    sp->SetFrame(color);
    sp->SetStopFrame(color);
	associated.AddComponent(sp);

    activationSound = new Sound(associated, HEALTHRUNE_SOUND);
    associated.AddComponent(activationSound);

    this->hpFactor = hpFactor;

    top_layer_sprite = nullptr;
    ressonance = nullptr;
}

void HealthRune::Update(float dt){

    static Timer cooldownTimer, ressonTimer;
    static bool active = true;
    
    if(Yawara::player){
        float dist = (associated.box.Center() - Yawara::player->GetCenterPos()).Modulo();
        InputManager& input = InputManager::GetInstance();

        if(dist <= HEALTHRUNE_ACTIVATION_DISTANCE && active && input.KeyPress(E_KEY)){
            Yawara::player->Boost(Yawara::HPBOOST, hpFactor);
            cooldownTimer.Restart();
            active = false;
            top_layer_sprite = new Sprite(associated, TOP_HEALTHRUNE_FILE);
            top_layer_sprite->SetScale(1, 1);
            associated.AddComponent(top_layer_sprite);
            activationSound->Play(1, 100);
            ressonance = new Sprite(associated, RESSONANCE_SPRITE, RESSONANCE_FRAME, RESSONANCE_FRAMETIME);
            ressonance->SetScale(0.6, 0.6);
            associated.AddComponent(ressonance);
            ressonTimer.Restart();
        }

        cooldownTimer.Update(dt);
        if(cooldownTimer.Get() >= HEALTHRUNE_COOLDOWN_TIME && !active){
            sp->SetBlendMode(BLEND_BLEND);
            active = true;
            associated.RemoveComponent(top_layer_sprite);
            top_layer_sprite = nullptr;
        }

        ressonTimer.Update(dt);
        if(ressonance && ressonTimer.Get() >= RESSONANCE_FRAME * RESSONANCE_FRAMETIME) {
            associated.RemoveComponent(ressonance);
            ressonance = nullptr;
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

void HealthRune::Render(){


}

bool HealthRune::Is(std::string type){

	return !std::min(strcmp(type.c_str(), "HealthRune"), strcmp(type.c_str(), "Item"));
}

void HealthRune::Start(){


}

void HealthRune::ChangeColor(Color color){

    Sprite* sp = static_cast<Sprite*> (associated.GetComponent("Sprite"));

    if(sp){
        sp->SetFrame(color);
        sp->SetStopFrame(color);
    }
}