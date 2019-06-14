#include "Item.h"
#include "Sprite.h"
#include "Yawara.h"

Item::Item(GameObject& associated) : Component(associated){


}

void Item::Update(float dt){


}

void Item::Render(){
    
}

bool Item::Is(std::string type){
    
	return !strcmp(type.c_str(), "Item");
}

void Item::Start(){
    
}
