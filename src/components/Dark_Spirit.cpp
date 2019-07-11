#include "Dark_Spirit.h"
#include "Sprite.h"
#include "Collider.h"
#include "Yawara.h"
#include "Bullet.h"
#include "Game.h"
#include "Hitbox.h"
#include "Capelobo.h"

#define SPRT_SPEED              250
#define SPRT_BULLET_SPEED       750
#define SPRT_BULLET_DAMAGE      5
#define SPRT_BULLET_DISTANCE    500
#define SPRT_BULLET_FRAMES      5
#define SPRT_BULLET_FM_TIME     0.05
#define REST_LIMIT              1.5
const std::string SPRT_BULLET_SOURCE = "assets/img/tapu/disparo.png";

#define SPRT_ENEMY_DIST_Y 250
#define SPRT_ENEMY_DIST_X 250

#define SPRT_DIST_DETECT_YAWARA     400
#define SPRT_DIST_OUT_OF_RANGE      600
#define SPRT_FREE_TIME_MOVEMENT     1
#define SPRT_PURSUE_MOVEMENT        2 
#define SPRT_LIMIT_TIME_MOVEMENT    1.5

// Movement sprites

const std::string MOVE_RIGHT 	  = "assets/img/dark_spirit/dark_spirit.png";
const std::string MOVE_RIGHT_DOWN = "assets/img/dark_spirit/dark_spirit.png";
const std::string MOVE_DOWN		  = "assets/img/dark_spirit/dark_spirit.png";
const std::string MOVE_LEFT_DOWN  = "assets/img/dark_spirit/dark_spirit.png";
const std::string MOVE_LEFT		  = "assets/img/dark_spirit/dark_spirit.png";
const std::string MOVE_LEFT_UP	  = "assets/img/dark_spirit/dark_spirit.png";
const std::string MOVE_UP		  = "assets/img/dark_spirit/dark_spirit.png";
const std::string MOVE_RIGHT_UP	  = "assets/img/dark_spirit/dark_spirit.png";

// Resting sprites

const std::string REST_RIGHT		= "assets/img/capelobo/capelobo_idle_right.png";
const std::string REST_RIGHT_DOWN 	= "assets/img/capelobo/capelobo_idle_right.png";
const std::string REST_DOWN		 	= "assets/img/capelobo/capelobo_idle_right.png";
const std::string REST_LEFT_DOWN	= "assets/img/capelobo/capelobo_idle_left.png";
const std::string REST_LEFT		 	= "assets/img/capelobo/capelobo_idle_left.png";
const std::string REST_LEFT_UP	 	= "assets/img/capelobo/capelobo_idle_left.png";
const std::string REST_UP		 	= "assets/img/capelobo/capelobo_idle_left.png";
const std::string REST_RIGHT_UP		= "assets/img/capelobo/capelobo_idle_right.png";

// Attacking sprites

const std::string ATTACK_RIGHT		= "assets/img/capelobo/capelobo_attack_r.png";
const std::string ATTACK_RIGHT_DOWN	= "assets/img/capelobo/capelobo_attack_r.png";
const std::string ATTACK_DOWN		= "assets/img/capelobo/capelobo_attack_r.png";
const std::string ATTACK_LEFT_DOWN	= "assets/img/capelobo/capelobo_attack_l.png";
const std::string ATTACK_LEFT		= "assets/img/capelobo/capelobo_attack_l.png";
const std::string ATTACK_LEFT_UP	= "assets/img/capelobo/capelobo_attack_l.png";
const std::string ATTACK_UP			= "assets/img/capelobo/capelobo_attack_r.png";
const std::string ATTACK_RIGHT_UP	= "assets/img/capelobo/capelobo_attack_r.png";

bool sprtStartedMoving = false;

Dark_Spirit::Dark_Spirit(GameObject & associated) : Enemy(associated){
    moveAllowed = true;

	hp = 70;
	speed = Vec2{0,0};
	Sprite *sp = new Sprite(associated, MOVE_DOWN, 4, 0.15);
	Collider *cl = new Collider(associated);
	associated.AddComponent(sp);
	associated.AddComponent(cl);

    state = MOVING;
    restOffset = 0;
}

Dark_Spirit::~Dark_Spirit(){
    
}

void Dark_Spirit::Update(float dt) {
    if (Yawara::player != nullptr)
	{
		yawaraPos = Yawara::player->GetCenterPos();
		switch (state)
		{
		case MOVING:
			moveTimer.Update(dt);
            if(moveTimer.Get() < SPRT_FREE_TIME_MOVEMENT){
                while(speed == Vec2{0,0}){
                    speed = Vec2{rand() % 2 ,rand() % 2 };
                    speed.x *= SPRT_SPEED;
                    speed.y *= SPRT_SPEED;
                    if(speed.x == speed.y)
                        speed /= 2;
                    Move45(speed); // Correct to move in 45 deg
                }

				moveAllowed = AllowedToMove(speed);
                
                if (!moveAllowed) // If he can't move, he will rest
                    speed = Vec2{0,0};
				// Move.
				else
				{
					associated.box.x += speed.x * dt;
					associated.box.y += speed.y * dt;
				}

            }
            else
            {
                speed = Vec2{0,0};
                state = RESTING;
                restTimer.Restart();
                break;
            }
            if((yawaraPos - associated.box.Center()).Modulo() <= SPRT_DIST_DETECT_YAWARA){
                state = PURSUE;
                rangeOffset = 0;
                restOffset = 1;
                moveTimer.Restart();
                sprtStartedMoving = true;
                break;
            }
            break;
        case PURSUE:
            moveTimer.Update(dt);
			if ((yawaraPos - associated.box.Center()).Modulo() < SPRT_DIST_OUT_OF_RANGE + rangeOffset && moveAllowed)
			{
				if ((yawaraPos - associated.box.Center()).Modulo() != 0)
					speed = ((yawaraPos - associated.box.Center()) / ((yawaraPos - associated.box.Center()).Modulo())) * SPRT_SPEED;
				else
					speed = {0, 0};

				// Yawara is nearby the enemy ore it moved for too long.
				if (moveTimer.Get() > SPRT_LIMIT_TIME_MOVEMENT || ((associated.box.Center().x <= SPRT_ENEMY_DIST_X + yawaraPos.x + dt * abs(speed.x)) && (associated.box.Center().x >= -SPRT_ENEMY_DIST_X + yawaraPos.x - dt * abs(speed.x)) &&
					 (associated.box.Center().y <= SPRT_ENEMY_DIST_Y + yawaraPos.y + dt * abs(speed.y)) && (associated.box.Center().y >= -SPRT_ENEMY_DIST_Y + yawaraPos.y - dt * abs(speed.y))) ||
					!moveAllowed || moveTimer.Get() > SPRT_PURSUE_MOVEMENT)
				{
					// Stop moving.
					speed.x = 0;
					speed.y = 0;

					// Change to BASIC_ATTACK state.
					// changed = false;
					state = BASIC_ATTACK;
					restTimer.Restart();
					moveTimer.Restart();
                    break;
				}

				// Correct root to move in 45 degrees
				Move45(speed);

				// Set dir to the direction that enemy is facing

				Direction lastDir = dir;

				if (speed.y > 10)
				{
					if (speed.x > 10)
						dir = RIGHT_DOWN;
					else if (speed.x < -10)
						dir = LEFT_DOWN;
					else
						dir = DOWN;
				}
				else if (speed.y < -10)
				{
					if (speed.x > 10)
						dir = RIGHT_UP;
					else if (speed.x < -10)
						dir = LEFT_UP;
					else
						dir = UP;
				}
				else
				{
					if (speed.x > 10)
						dir = RIGHT;
					else if (speed.x < -10)
						dir = LEFT;
				}
				if (dir != lastDir || sprtStartedMoving){
					change_sprite = true;
					sprtStartedMoving = false;
				}

				moveAllowed = AllowedToMove(speed);
				
				if (!moveAllowed) // If he can't move, he will rest
				{
					state = RESTING;
					restTimer.Restart();
				}
				// Move.
				else
				{
					associated.box.x += speed.x * dt;
					associated.box.y += speed.y * dt;
				}
			}
			else if((yawaraPos - associated.box.Center()).Modulo() > SPRT_DIST_DETECT_YAWARA){
				state = SLEEPING;
			}
			break;

		case RESTING:
			restTimer.Update(dt);
			if (!change_sprite)
				change_sprite = true;

			// Rest for a determinated time
			if (restTimer.Get() > REST_LIMIT - restOffset)
			{
                if((yawaraPos - associated.box.Center()).Modulo() <= SPRT_DIST_DETECT_YAWARA){
                    state = PURSUE;
                    rangeOffset = 0;
                    restOffset = 1;
                    moveTimer.Restart();
                    sprtStartedMoving = true;
                    break;
                }
				state = MOVING;
                restOffset = 0;
				moveAllowed = 1;
				moveTimer.Restart();
			}
			break;

		case BASIC_ATTACK:
            attackTimer.Update(dt);
            if (/*!startedAttack && */ attackTimer.Get() > 0.3)
			{
                GameObject *bullet_GO = new GameObject;
                std::weak_ptr<GameObject> weak_bullet = Game::GetInstance().GetCurrentState().AddObject(bullet_GO);
                std::shared_ptr<GameObject> ptr = weak_bullet.lock();

                float angle = (yawaraPos - associated.box.Center()).Inclination();
                
				if (angle <= 0)
					angle += 360;
                    
                if(angle >= 337.5 || angle < 22.5){
					angle = 360;
					dir = RIGHT;
				}
				else if(angle >= 22.5 && angle < 67.5){
					angle = 45;
					dir = RIGHT_DOWN;	
				}
				else if(angle >= 67.5 && angle < 112.5){
					angle = 90;
					dir = DOWN;
				}
				else if(angle >= 112.5 && angle < 157.5){
					angle = 135;
					dir = LEFT_DOWN;
				}
				else if(angle >= 157.5 && angle < 202.5){
					angle = 180;
					dir = LEFT;
				}
				else if(angle >= 202.5 && angle < 247.5){
					angle = 225;
					dir = LEFT_UP;
				}
				else if(angle >= 247.5 && angle < 292.5){
					angle = 270;
					dir = UP;
				}
				else{
					angle = 315;
					dir = RIGHT_UP;
				}

                angle *= PI/180;

                Bullet *bullet = new Bullet(*ptr, angle, SPRT_BULLET_SPEED, SPRT_BULLET_DAMAGE, SPRT_BULLET_DISTANCE, SPRT_BULLET_SOURCE, SPRT_BULLET_FRAMES, SPRT_BULLET_FM_TIME, true);
                ptr->box.Centered(associated.box.Center());
                Vec2 offset(associated.box.w / 2, 0);
                offset.Rotate(angle);
                ptr->box.x += offset.x;
                ptr->box.y += offset.y;

                ptr->AddComponent(bullet);
                state = RESTING;
                restTimer.Restart();
            }
			break;
        default:
            break;
		}
        if(hp <= 0){
            state = RESTING;
            deathTimer.Update(dt);
            
            GameObject *go = new GameObject();
            std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
            std::shared_ptr<GameObject> ptr = weak_ptr.lock();
            Sprite *sp;

            if(deathTimer.Get() < DEATH_EFFECT_TIME){}
            else{
                if(Capelobo::boss != nullptr){
                    GameObject *goSpirit = new GameObject();
                    std::weak_ptr<GameObject> weak_ptr_sprt = Game::GetInstance().GetCurrentState().AddObject(goSpirit);
                    std::shared_ptr<GameObject> ptr_sprt = weak_ptr_sprt.lock();
                    Dark_Spirit *sprt = new Dark_Spirit(*ptr_sprt);
                    ptr_sprt->box.Centered({associated.box.x + rand()%2500 - rand()%2500, associated.box.y + rand()%500 - rand()%500});
                    if(ptr_sprt->box.Center().x < 0)
                        ptr_sprt->box.Centered({associated.box.x + rand()%2500, ptr_sprt->box.Center().y});
                    if(ptr_sprt->box.Center().y < 0)
                        ptr_sprt->box.Centered({ptr_sprt->box.Center().x, associated.box.y + rand()%2500});
                    ptr_sprt->AddComponent(sprt);
                }
                associated.RequestDelete();
				// sp = new Sprite(*ptr, DEATH_RIGHT, 9, 0.1, 9 * 0.1);
                // Sound *so = new Sound(*ptr, "assets/audio/boom.wav");
                // ptr->AddComponent(so);

                // so->Play(1);
            }
        }
	}
}

void Dark_Spirit::Render() {
    Vec2 position = associated.box.Center();

}

void Dark_Spirit::NotifyCollision(GameObject& other){
    if (other.GetComponent("Bullet") && !static_cast<Bullet *>(other.GetComponent("Bullet"))->targetsPlayer){
		hp -= static_cast<Bullet *>(other.GetComponent("Bullet"))->GetDamage();
        
        //  If the enemy can't see Yawara and got hit, it will enhance the range off vision and start pursuing Yawara

        if(state == MOVING || state == RESTING){
            rangeOffset = 300;
            state = PURSUE;
            moveTimer.Restart();
            sprtStartedMoving = true;
        }
    }
	Hitbox *hitbox = static_cast<Hitbox *>(other.GetComponent("Hitbox"));
	if (hitbox && !(hitbox->targetsPlayer) && hitTimer.Get() > HIT_COOL_DOWN) {
		hp -= hitbox->GetDamage();
		hitTimer.Restart();
	}
}

bool Dark_Spirit::Is(std::string type){
	return !std::min(strcmp(type.c_str(), "Dark_Spirit"),strcmp(type.c_str(), "Enemy"));
}
