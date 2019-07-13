#include "Enemy.h"
#include "string.h"
#include "Hitbox.h"
#include "Bullet.h"
#include "Floor.h"
#include "Game.h"
#include "Sound.h"
#include <algorithm> 

#define SAFE_UP 16
#define SAFE_DOWN 142
#define SAFE_SIDE 128

#define HIT_COOL_DOWN 1

Enemy::Enemy(GameObject& associated) : Component(associated){
    change_sprite = false;
}

Enemy::~Enemy(){

}

void Enemy::Start(){
    // Reset all timers
	restTimer.Restart();
	moveTimer.Restart();
	attackTimer.Restart();
	hitTimer.Restart();
    
    dir = RIGHT;
}

void Enemy::Update(float dt){

}

void Enemy::Render(){

}

bool Enemy::Is(std::string type){
    return !strcmp(type.c_str(), "Enemy");
}


void Enemy::NotifyCollision(GameObject& other){
	Hitbox *hitbox = static_cast<Hitbox *>(other.GetComponent("Hitbox"));
	if (hitbox && !(hitbox->targetsPlayer) && hitTimer.Get() > HIT_COOL_DOWN) {
        hp -= hitbox->GetDamage();
		hitTimer.Restart();
        HitSound();
	}
    if (other.GetComponent("Bullet") && !static_cast<Bullet *>(other.GetComponent("Bullet"))->targetsPlayer){
        hp -= static_cast<Bullet *>(other.GetComponent("Bullet"))->GetDamage();
		hitTimer.Restart();
        HitSound();        
        //  If the enemy can't see Yawara and got hit, it will enhance the range off vision and start pursuing Yawara
        if(state == MOVING || state == RESTING || state == SLEEPING){
            rangeOffset = 300;
            state = PURSUE;
            moveTimer.Restart();
        }
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

void Enemy::Move45(Vec2& speed){
    if (abs(speed.x) > abs(speed.y) && (speed.y < -10 || speed.y > 10))
    {
        int signalY = speed.y / abs(speed.y);
        int signalX = speed.x / abs(speed.x);
        speed = {speed.x, signalY * signalX * speed.x};
    }
    else if (abs(speed.x) < abs(speed.y) && (speed.x < -10 || speed.x > 10))
    {
        int signalY = speed.y / abs(speed.y);
        int signalX = speed.x / abs(speed.x);
        speed = {signalY * signalX * speed.y, speed.y};
    }
}

void Enemy::HitSound(){
}