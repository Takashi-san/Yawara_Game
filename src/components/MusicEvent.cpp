#include "MusicEvent.h"
#include "Yawara.h"
#include "Dark_Spirit.h"
#include "Game.h"
#include "Collider.h"

MusicEvent::MusicEvent(GameObject& associated, float x, float y, float w, float h, std::string musica) : Component(associated) {

    associated.box.x = x;
    associated.box.y = y;
    associated.box.w = w;
    associated.box.h = h;

    this->musica = musica;
}

void MusicEvent::Update(float dt) {
    static bool flag = true;
    if(Yawara::player){
        if(associated.box.Within(Yawara::player->GetCenterPos().x, Yawara::player->GetCenterPos().y) && flag){
            bgMusic.Open(musica.c_str());
            bgMusic.Play(-1);
            flag = false;
        }
    }
}

void MusicEvent::Render() {

}

bool MusicEvent::Is(std::string type) {
    return !strcmp(type.c_str(), "MusicEvent");
}

void MusicEvent::NotifyCollision(GameObject &) {

}

void MusicEvent::Start() {

}