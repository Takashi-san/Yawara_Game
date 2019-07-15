#include "Event.h"
#include "Yawara.h"
#include "Dark_Spirit.h"
#include "Game.h"
#include "Collider.h"

Event::Event(GameObject& associated, float x, float y, float w, float h) : Component(associated) {

    associated.box.x = x;
    associated.box.y = y;
    associated.box.w = w;
    associated.box.h = h;
}

void Event::Update(float dt) {
    if(Yawara::player){
        if(associated.box.Within(Yawara::player->GetCenterPos().x, Yawara::player->GetCenterPos().y)){
            GameObject* enemygo = new GameObject();
            std::weak_ptr<GameObject> weak_ptr;
            std::shared_ptr<GameObject> ptr;
            weak_ptr = Game::GetInstance().GetCurrentState().AddObject(enemygo);
            ptr = weak_ptr.lock();

            Dark_Spirit* dark = new Dark_Spirit(*ptr);
            ptr->AddComponent(dark);

            ptr->box.x = associated.box.x + rand()%((int) associated.box.w);
            ptr->box.y = associated.box.y + rand()%((int) associated.box.h);
            
            associated.RequestDelete();
        }
    }
}

void Event::Render() {

}

bool Event::Is(std::string type) {
    return !strcmp(type.c_str(), "Event");
}

void Event::NotifyCollision(GameObject &) {

}

void Event::Start() {

}