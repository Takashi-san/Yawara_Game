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
#define SPRT_BULLET_FRAMES      6
#define SPRT_BULLET_FM_TIME     0.05
#define REST_LIMIT              1.5


#define SPRT_ENEMY_DIST_Y 250
#define SPRT_ENEMY_DIST_X 250

#define SPRT_DIST_DETECT_YAWARA     400
#define SPRT_DIST_OUT_OF_RANGE      600
#define SPRT_FREE_TIME_MOVEMENT     1
#define SPRT_PURSUE_MOVEMENT        2 
#define SPRT_LIMIT_TIME_MOVEMENT    1.5

// Movement sprites

const std::string MOVE_RIGHT 	  = "assets/img/dark_spirit/Minion roxlo lado direito.png";
const std::string MOVE_RIGHT_DOWN = "assets/img/dark_spirit/Minion roxlo lado direito.png";
const std::string MOVE_DOWN		  = "assets/img/dark_spirit/Minion roxo frente.png";
const std::string MOVE_LEFT_DOWN  = "assets/img/dark_spirit/Minion roxo frente.png";
const std::string MOVE_LEFT		  = "assets/img/dark_spirit/Minion roxlo lado esquerdo.png";
const std::string MOVE_LEFT_UP	  = "assets/img/dark_spirit/Minion roxlo lado esquerdo.png";
const std::string MOVE_UP		  = "assets/img/dark_spirit/Minion roxo costa.png";
const std::string MOVE_RIGHT_UP	  = "assets/img/dark_spirit/Minion roxo costa.png";

// Sounds

const std::string SPRT_HIT_SOUND	= "assets/audio/sons/spirit/hit.ogg";
const std::string SPRT_B_ATK_SOUND	= "assets/audio/sons/spirit/ataque.ogg";
const std::string SPRT_DEATH		= "assets/audio/sons/spirit/morte.ogg";
const std::string SPRT_SHOOT_SOUND	= "assets/audio/sons/spirit/disparo.ogg";

// Bullet sprites

const std::string SPRT_BULLET_SOURCE = "assets/img/dark_spirit/efeito_disparo_inimigo.png";
const std::string SPRT_BULLET_END 	= "assets/img/dark_spirit/disparo_destruido.png";

// Death sprites

const std::string DEATH_RIGHT		= "assets/img/capelobo/capelobo_morte_r.png";
const std::string DEATH_LEFT	 	= "assets/img/capelobo/capelobo_morte_l.png";
const std::string EFFECT_1			= "assets/img/capelobo/camada1_efeitomorteboss.png";
const std::string EFFECT_2			= "assets/img/capelobo/camada2_efeitomorteboss.png";
const std::string EFFECT_3			= "assets/img/capelobo/camada3_efeitomorteboss.png";
const std::string EFFECT_4			= "assets/img/capelobo/camada4_efeitomorteboss.png";
const std::string EFFECT_5			= "assets/img/capelobo/camada5_efeitomorteboss.png";


bool sprtStartedMoving = false;
int sprttimesPlayed = 0;

Dark_Spirit::Dark_Spirit(GameObject & associated) : Enemy(associated){
    moveAllowed = true;
	shoot = new Sound(associated, SPRT_SHOOT_SOUND);
	timesPlayed = 0;

	hp = 40;
	speed = Vec2{0,0};
	Sprite *sp = new Sprite(associated, MOVE_DOWN, 7, 0.15);
	sp->SetScale({1.5,1.5});
	Collider *cl = new Collider(associated);
	associated.AddComponent(sp);
	associated.AddComponent(cl);

    state = MOVING;
    restOffset = 0;
	alpha = 225;
}

Dark_Spirit::~Dark_Spirit(){
    
}

void Dark_Spirit::Update(float dt) {

    hitTimer.Update(dt);
	change_sprite = false;

    if (Yawara::player != nullptr)
	{
		yawaraPos = Yawara::player->GetCenterPos();
		switch (state)
		{
		case MOVING:
			moveTimer.Update(dt);
            if(moveTimer.Get() < SPRT_FREE_TIME_MOVEMENT && !((yawaraPos - associated.box.Center()).Modulo() <= SPRT_DIST_DETECT_YAWARA + rangeOffset)){
                while(speed == Vec2{0,0}){
                    speed = Vec2{rand() % 2 ,rand() % 2 };
                    speed.x *= SPRT_SPEED;
                    speed.y *= SPRT_SPEED;
                    if(speed.x == speed.y)
                        speed /= 2;
                    Move45(speed); // Correct to move in 45 deg
                }

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
                    speed = Vec2{0,0};
				// Move.
				else
				{
					associated.box.x += speed.x * dt;
					associated.box.y += speed.y * dt;
				}

            }
            else if((yawaraPos - associated.box.Center()).Modulo() <= SPRT_DIST_DETECT_YAWARA + rangeOffset){
                state = PURSUE;
                rangeOffset = 0;
                restOffset = 1;
                moveTimer.Restart();
                sprtStartedMoving = true;
            }
            else
            {
                speed = Vec2{0,0};
                state = RESTING;
                restTimer.Restart();
            }
            break;
        case PURSUE:
            sprtStartedMoving = true;
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
					timesPlayed = 0;
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
			else{
				state = RESTING;
			}
			break;

		case RESTING:
			restTimer.Update(dt);
			change_sprite = false;

			// Rest for a determinated time
			if (restTimer.Get() > REST_LIMIT - restOffset)
			{
                if((yawaraPos - associated.box.Center()).Modulo() <= SPRT_DIST_DETECT_YAWARA + rangeOffset){
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
            if (attackTimer.Get() > 1.5)
			{
                GameObject *bullet_GO = new GameObject;
                std::weak_ptr<GameObject> weak_bullet = Game::GetInstance().GetCurrentState().AddObject(bullet_GO);
                std::shared_ptr<GameObject> ptr = weak_bullet.lock();

                float angle = (yawaraPos - associated.box.Center()).Inclination();
                if(timesPlayed == 0){
					shoot->Play(1, MIX_MAX_VOLUME);
					++timesPlayed;
				}
				if (angle <= 0)
					angle += 360;
                    
				Direction lastDir = dir;

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

				if (dir != lastDir || sprtStartedMoving)
					change_sprite = true;

                angle *= PI/180;

                Bullet *bullet = new Bullet(*ptr, angle, SPRT_BULLET_SPEED, SPRT_BULLET_DAMAGE, SPRT_BULLET_DISTANCE, SPRT_BULLET_SOURCE, SPRT_BULLET_END, SPRT_BULLET_FRAMES, SPRT_BULLET_FM_TIME, true);
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
			state = RESTING;
			restTimer.Restart();
            break;
		}
        if(hp <= 0){
            state = RESTING;
            deathTimer.Update(dt);
            

			GameObject *go = new GameObject();
            std::weak_ptr<GameObject> weak_sound = Game::GetInstance().GetCurrentState().AddObject(go);
            std::shared_ptr<GameObject> ptr_sound = weak_sound.lock();
			Sound *so = new Sound(*ptr_sound, SPRT_DEATH);
			Sprite *sp = static_cast<Sprite *>(associated.GetComponent("Sprite"));
			ptr_sound->AddComponent(so);

			if(sprttimesPlayed == 0){
				so->Play(1, MIX_MAX_VOLUME);
				++sprttimesPlayed;
			}
			if(deathTimer.Get() < DEATH_EFFECT_TIME){
				if(alpha > 0)
					alpha -= dt;
				sp->SetAlphaMod(alpha);
			}

			associated.RequestDelete();
        }
	}
}

void Dark_Spirit::Render() {
    Vec2 position = associated.box.Center();
	Sprite *sp = static_cast<Sprite *>(associated.GetComponent("Sprite"));
	if (change_sprite && sp)
	{
		change_sprite = false;
		sp->SetFrameTime(0.15);
		switch (dir)
		{
		case RIGHT:
			sp->Open(MOVE_RIGHT);
			sp->SetFrameCount(7);
			break;

		case LEFT:
			sp->Open(MOVE_LEFT);
			sp->SetFrameCount(7);
			break;

		case UP:
			sp->Open(MOVE_UP);
			sp->SetFrameCount(7);
			break;

		case DOWN:
			sp->Open(MOVE_DOWN);
			sp->SetFrameCount(7);
			break;

		case RIGHT_UP:
			sp->Open(MOVE_RIGHT_UP);
			sp->SetFrameCount(7);
			break;

		case RIGHT_DOWN:
			sp->Open(MOVE_RIGHT_DOWN);
			sp->SetFrameCount(7);
			break;

		case LEFT_UP:
			sp->Open(MOVE_LEFT_UP);
			sp->SetFrameCount(7);
			break;

		case LEFT_DOWN:
			sp->Open(MOVE_LEFT_DOWN);
			sp->SetFrameCount(7);
			break;

		default:
			break;
		}
	}
	associated.box.Centered(position);
}

bool Dark_Spirit::Is(std::string type){
	return !std::min(strcmp(type.c_str(), "Dark_Spirit"),strcmp(type.c_str(), "Enemy"));
}

void Dark_Spirit::HitSound(){
    GameObject *soundGO = new GameObject();
    std::weak_ptr<GameObject> weak_hit = Game::GetInstance().GetCurrentState().AddObject(soundGO);
    std::shared_ptr<GameObject> shared_hit = weak_hit.lock();
    Sound *so = new Sound(*shared_hit, SPRT_HIT_SOUND);
    shared_hit->AddComponent(so);
    so->Play(1,MIX_MAX_VOLUME);
}