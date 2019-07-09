#include "Enemy.h"
#include "string.h"
#include "Hitbox.h"
#include "Bullet.h"
#include "Floor.h"

#define SAFE_UP 16
#define SAFE_DOWN 142
#define SAFE_SIDE 128

#define HIT_COOL_DOWN 1

Enemy::Enemy(GameObject& associated) : Component(associated){

}

Enemy::~Enemy(){

}

void Enemy::Start(){

}

void Enemy::Update(float dt){

}

void Enemy::Render(){

}

bool Enemy::Is(std::string type){
    return !strcmp(type.c_str(), "Enemy");
}


void Enemy::NotifyCollision(GameObject& other){
    if (other.GetComponent("Bullet") && !static_cast<Bullet *>(other.GetComponent("Bullet"))->targetsPlayer)
		hp -= static_cast<Bullet *>(other.GetComponent("Bullet"))->GetDamage();

	Hitbox *hitbox = static_cast<Hitbox *>(other.GetComponent("Hitbox"));
	if (hitbox && !(hitbox->targetsPlayer) && hitTimer.Get() > HIT_COOL_DOWN) {
		hp -= hitbox->GetDamage();
		hitTimer.Restart();
	}
}


bool Enemy::AllowedToMove(Vec2 speed){
    // Verify if enemy is allowed to move

    if (speed.y > 0.1)
    {
        safeY = associated.box.Center().y + SAFE_UP + SAFE_DOWN;
    }
    else if (speed.y < -0.1)
        safeY = associated.box.Center().y + SAFE_UP;
    else
        safeY = associated.box.Center().y + SAFE_DOWN;

    if (speed.x > 0.1)
        safeX = associated.box.Center().x + SAFE_SIDE;
    else if (speed.x < -0.1)
        safeX = associated.box.Center().x - SAFE_SIDE;
    else
        safeX = associated.box.Center().x;

    // return (Floor::loaded && Floor::AtAllowedArea(safeX, safeY, 0));
    return 1;
}
