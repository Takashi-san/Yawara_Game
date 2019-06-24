#include "NPC.h"
#include "InputManager.h"
#include "Yawara.h"

#define MIN_DIST 80 //Minimal distance for interaction

NPC::NPC(GameObject associated, bool interactive) : Component(associated) {

    this->interactive = interactive;
}

/* Minimal Update function for NPCs */
void NPC::Update(float dt) {
    InputManager& input = InputManager::GetInstance();

    if(input.KeyPress(SPACE_KEY)){

        float dist = (associated.box.Center() - Yawara::player->GetPos()).Modulo();
        if(interactive && dist <= MIN_DIST){
            Action();
        }
    }
}

void NPC::Action() {

 /* Must be implemented in derived classes */
}