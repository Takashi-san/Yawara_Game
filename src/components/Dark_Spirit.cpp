#include "Dark_Spirit.h"
#include "Sprite.h"
#include "Collider.h"
#include "Yawara.h"
#include "Bullet.h"
#include "Game.h"

#define SPRT_SPEED              250
#define SPRT_BULLET_SPEED       750
#define SPRT_BULLET_DAMAGE      5
#define SPRT_BULLET_DISTANCE    500
#define SPRT_BULLET_FRAMES      5
#define SPRT_BULLET_FM_TIME     0.05
#define REST_LIMIT              1.5
const std::string SPRT_BULLET_SOURCE = "assets/img/tapu/disparo.png";

#define SPRT_ENEMY_DIST_Y 50
#define SPRT_ENEMY_DIST_X 50

#define SPRT_DIST_DETECT_YAWARA 400
#define SPRT_DIST_OUT_OF_RANGE 600
#define SPRT_FREE_TIME_MOVEMENT 1
#define SPRT_LIMIT_TIME_MOVEMENT 1.5

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
                restOffset = 1;
                moveTimer.Restart();
                sprtStartedMoving = true;
                break;
            }
            break;
        case PURSUE:
            moveTimer.Update(dt);
			if ((yawaraPos - associated.box.Center()).Modulo() < SPRT_DIST_OUT_OF_RANGE && moveAllowed)
			{
				if ((yawaraPos - associated.box.Center()).Modulo() != 0)
					speed = ((yawaraPos - associated.box.Center()) / ((yawaraPos - associated.box.Center()).Modulo())) * SPRT_SPEED;
				else
					speed = {0, 0};

				// Yawara is nearby the Capelobo.
				if (moveTimer.Get() > SPRT_LIMIT_TIME_MOVEMENT || ((associated.box.Center().x <= SPRT_ENEMY_DIST_X + yawaraPos.x + dt * abs(speed.x)) && (associated.box.Center().x >= -SPRT_ENEMY_DIST_X + yawaraPos.x - dt * abs(speed.x)) &&
					 (associated.box.Center().y <= SPRT_ENEMY_DIST_Y + yawaraPos.y + dt * abs(speed.y)) && (associated.box.Center().y >= -SPRT_ENEMY_DIST_Y + yawaraPos.y - dt * abs(speed.y))) ||
					!moveAllowed)
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

                int angle = (yawaraPos - associated.box.Center()).Inclination();
				std::cout << angle << '\t';
                angle = angle + (45 - angle % 45);
				if (angle <= 0)
					angle += 360;
                if (angle > 360)
                    angle -= 360;

                std::cout << angle << std::endl;

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
	}
}

void Dark_Spirit::Render() {
    Vec2 position = associated.box.Center();

	// Sprite *sp = static_cast<Sprite *>(associated.GetComponent("Sprite"));
	// if (change_sprite && sp)
	// {
	// 	change_sprite = false;
		// if (state == MOVING)
		// {
		// 	sp->SetFrameTime(0.1);
		// 	switch (dir)
		// 	{
		// 	case RIGHT:
		// 		sp->Open(MOVE_RIGHT);
		// 		sp->SetFrameCount(12);
		// 		break;

		// 	case LEFT:
		// 		sp->Open(MOVE_LEFT);
		// 		sp->SetFrameCount(12);
		// 		break;

		// 	case UP:
		// 		sp->Open(MOVE_UP);
		// 		sp->SetFrameCount(12);
		// 		break;

		// 	case DOWN:
		// 		sp->Open(MOVE_DOWN);
		// 		sp->SetFrameCount(12);
		// 		break;

		// 	case RIGHT_UP:
		// 		sp->Open(MOVE_RIGHT_UP);
		// 		sp->SetFrameCount(12);
		// 		break;

		// 	case RIGHT_DOWN:
		// 		sp->Open(MOVE_RIGHT_DOWN);
		// 		sp->SetFrameCount(12);
		// 		break;

		// 	case LEFT_UP:
		// 		sp->Open(MOVE_LEFT_UP);
		// 		sp->SetFrameCount(12);
		// 		break;

		// 	case LEFT_DOWN:
		// 		sp->Open(MOVE_LEFT_DOWN);
		// 		sp->SetFrameCount(12);
		// 		break;

		// 	default:
		// 		break;
		// 	}
		// }
		// else if (state == RESTING)
		// {
		// 	sp->SetFrameTime(0.15);
		// 	switch (dir)
		// 	{
		// 	case RIGHT:
		// 		sp->Open(REST_RIGHT);
		// 		sp->SetFrameCount(8);
		// 		break;

		// 	case LEFT:
		// 		sp->Open(REST_LEFT);
		// 		sp->SetFrameCount(8);
		// 		break;

		// 	case UP:
		// 		sp->Open(REST_UP);
		// 		sp->SetFrameCount(8);
		// 		break;

		// 	case DOWN:
		// 		sp->Open(REST_DOWN);
		// 		sp->SetFrameCount(8);
		// 		break;

		// 	case RIGHT_UP:
		// 		sp->Open(REST_RIGHT_UP);
		// 		sp->SetFrameCount(8);
		// 		break;

		// 	case RIGHT_DOWN:
		// 		sp->Open(REST_RIGHT_DOWN);
		// 		sp->SetFrameCount(8);
		// 		break;

		// 	case LEFT_UP:
		// 		sp->Open(REST_LEFT_UP);
		// 		sp->SetFrameCount(8);
		// 		break;

		// 	case LEFT_DOWN:
		// 		sp->Open(REST_LEFT_DOWN);
		// 		sp->SetFrameCount(8);
		// 		break;

		// 	default:
		// 		break;
		// 	}
		// }
		// else if (state == BASIC_ATTACK)
		// {
		// 	sp->SetFrameTime(0.05);
		// 	switch (dir)
		// 	{
		// 	case RIGHT:
		// 		sp->Open(ATTACK_RIGHT);
		// 		sp->SetFrameCount(16);
		// 		break;

		// 	case LEFT:
		// 		sp->Open(ATTACK_LEFT);
		// 		sp->SetFrameCount(16);
		// 		break;

		// 	case UP:
		// 		sp->Open(ATTACK_UP);
		// 		sp->SetFrameCount(16);
		// 		break;

		// 	case DOWN:
		// 		sp->Open(ATTACK_DOWN);
		// 		sp->SetFrameCount(16);
		// 		break;

		// 	case RIGHT_UP:
		// 		sp->Open(ATTACK_RIGHT_UP);
		// 		sp->SetFrameCount(16);
		// 		break;

		// 	case RIGHT_DOWN:
		// 		sp->Open(ATTACK_RIGHT_DOWN);
		// 		sp->SetFrameCount(16);
		// 		break;

		// 	case LEFT_UP:
		// 		sp->Open(ATTACK_LEFT_UP);
		// 		sp->SetFrameCount(16);
		// 		break;

		// 	case LEFT_DOWN:
		// 		sp->Open(ATTACK_LEFT_DOWN);
		// 		sp->SetFrameCount(16);
		// 		break;

		// 	default:
		// 		break;
		// 	}
		// }
		// else if(state == SLEEPING){
			// sp->SetFrameTime(0.08);
			// sp->Open(LOAD_LEFT_DOWN);
			// sp->SetFrameCount(4);
	// 	}
	// }
	// associated.box.Centered(position);
}

bool Dark_Spirit::Is(std::string type){
    return !strcmp(type.c_str(), "Dark_Spirit");
}
