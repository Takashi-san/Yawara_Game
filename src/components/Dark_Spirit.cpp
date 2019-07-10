#include "Dark_Spirit.h"
#include "Sprite.h"
#include "Collider.h"
#include "Yawara.h"

#define SPRT_SPEED 250

#define DIST_DETECT_YAWARA 400
#define FREE_TIME_MOVEMENT 1
#define LIMIT_TIME_MOVEMENT 1.5

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

Dark_Spirit::Dark_Spirit(GameObject & associated) : Enemy(associated){
    moveAllowed = true;

	hp = 70;
	speed = Vec2{0,0};
	Sprite *sp = new Sprite(associated, MOVE_DOWN, 4, 0.15);
	Collider *cl = new Collider(associated);
	associated.AddComponent(sp);
	associated.AddComponent(cl);

    state = MOVING;
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
            if(moveTimer.Get() < FREE_TIME_MOVEMENT){
                while(speed == Vec2{0,0}){
                    speed = Vec2{rand() % 2 ,rand() % 2 };
                    std::cout << speed.x << '\t' << speed.y << std::endl;
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
            }
            
			// else if ((yawaraPos - associated.box.Center()).Modulo() < 900 && moveAllowed)
			// {

			// 	if(moveTimer.Get() > LIMIT_TIME_MOVEMENT && (yawaraPos - associated.box.Center()).Modulo() <= DIST_LOAD_ATTACK)
			// 	{
			// 		// Stop moving.
			// 		speed.x = 0;
			// 		speed.y = 0;

			// 		// Change to LOAD_ATTACK state.
			// 		startedAttack = false;
			// 		changed = false;
			// 		state = LOAD_ATTACK;
			// 		restTimer.Restart();
			// 		moveTimer.Restart();
			// 	}

			// 	if ((yawaraPos - associated.box.Center()).Modulo() != 0)
			// 		speed = ((yawaraPos - associated.box.Center()) / ((yawaraPos - associated.box.Center()).Modulo())) * BOSS_SPEED;
			// 	else
			// 		speed = {0, 0};

			// 	// Yawara is nearby the Capelobo.
			// 	if (((associated.box.Center().x <= CPLB_ENEMY_DIST_X + yawaraPos.x + dt * abs(speed.x)) && (associated.box.Center().x >= -CPLB_ENEMY_DIST_X + yawaraPos.x - dt * abs(speed.x)) &&
			// 		 (associated.box.Center().y <= CPLB_ENEMY_DIST_Y + yawaraPos.y + dt * abs(speed.y)) && (associated.box.Center().y >= -CPLB_ENEMY_DIST_Y + yawaraPos.y - dt * abs(speed.y))) ||
			// 		!moveAllowed)
			// 	{
			// 		// Stop moving
			// 		speed.x = 0;
			// 		speed.y = 0;

			// 		// Change to BASIC_ATTACK state.
			// 		startedAttack = false;
			// 		changed = false;
			// 		state = BASIC_ATTACK;
			// 		break;
			// 	}

			// 	// Correct root to move in 45 degrees
			// 	if (abs(speed.x) > abs(speed.y) && (speed.y < -10 || speed.y > 10))
			// 	{
			// 		int signalY = speed.y / abs(speed.y);
			// 		int signalX = speed.x / abs(speed.x);
			// 		speed = {speed.x, signalY * signalX * speed.x};
			// 	}
			// 	else if (abs(speed.x) < abs(speed.y) && (speed.x < -10 || speed.x > 10))
			// 	{
			// 		int signalY = speed.y / abs(speed.y);
			// 		int signalX = speed.x / abs(speed.x);
			// 		speed = {signalY * signalX * speed.y, speed.y};
			// 	}

			// 	// Set dir to the direction that Capelobo is facing

			// 	Direction lastDir = dir;

			// 	if (speed.y > 10)
			// 	{
			// 		if (speed.x > 10)
			// 			dir = RIGHT_DOWN;
			// 		else if (speed.x < -10)
			// 			dir = LEFT_DOWN;
			// 		else
			// 			dir = DOWN;
			// 	}
			// 	else if (speed.y < -10)
			// 	{
			// 		if (speed.x > 10)
			// 			dir = RIGHT_UP;
			// 		else if (speed.x < -10)
			// 			dir = LEFT_UP;
			// 		else
			// 			dir = UP;
			// 	}
			// 	else
			// 	{
			// 		if (speed.x > 10)
			// 			dir = RIGHT;
			// 		else if (speed.x < -10)
			// 			dir = LEFT;
			// 	}
			// 	if (dir != lastDir || startedMoving){
			// 		change_sprite = true;
			// 		startedMoving = false;
			// 	}

			// 	moveAllowed = AllowedToMove(speed);
				
			// 	if (!moveAllowed) // If he can't move, he will rest
			// 	{
			// 		state = RESTING;
			// 		restTimer.Restart();
			// 	}
			// 	// Move.
			// 	else
			// 	{
			// 		associated.box.x += speed.x * dt;
			// 		associated.box.y += speed.y * dt;
			// 	}
			// }
			// else if((yawaraPos - associated.box.Center()).Modulo() > DIST_DETECT_YAWARA){
			// 	state = SLEEPING;
			// }
			break;

		case RESTING:
			restTimer.Update(dt);
			if (!change_sprite)
				change_sprite = true;

			// Rest for a determinated time
			if (restTimer.Get() > 3)
			{
				// if ((yawaraPos - associated.box.Center()).Modulo() != 0)
				// 	speed = ((yawaraPos - associated.box.Center()) / ((yawaraPos - associated.box.Center()).Modulo())) * SPRT_SPEED;
				// else
				// 	speed = {0, 0};

				// temp_speed = speed;

				state = MOVING;
				// startedMoving = true;
				moveAllowed = 1;
				moveTimer.Restart();
			}
			break;

		// case BASIC_ATTACK:
		// 	attackTimer.Update(dt);

		// 	// Make change sprite just when the attack start and if it hasn't changed yet
		// 	if (change_sprite && !startedAttack){
		// 		change_sprite = false;
		// 		changed = false;
		// 	}

		// 	if (!change_sprite && !changed){
		// 		change_sprite = true;
		// 		changed = true;
		// 	}

		// 	// Create Hitbox if it wasn't created yet. Create it 0.3s after render the sprite (time that he actualy attack on the sprite)
		// 	if (!startedAttack && attackTimer.Get() > 0.3)
		// 	{
		// 		// Create first Claw hitbox
		// 		GameObject *clawGO = new GameObject();
		// 		std::weak_ptr<GameObject> weak_claw1 = Game::GetInstance().GetCurrentState().AddObject(clawGO);
		// 		std::shared_ptr<GameObject> shared_claw1 = weak_claw1.lock();
		// 		Claw *theClaw1;

		// 		// Create second Claw hitbox. It'll be used when diagonal attack
		// 		GameObject *clawGO2 = new GameObject();
		// 		std::weak_ptr<GameObject> weak_claw2 = Game::GetInstance().GetCurrentState().AddObject(clawGO2);
		// 		std::shared_ptr<GameObject> shared_claw2 = weak_claw2.lock();
		// 		Claw *theClaw2;

		// 		startedAttack = true;

		// 		std::weak_ptr<GameObject> weak_Boss = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);

		// 		int angle = (Yawara::player->GetCenterPos() - associated.box.Center()).Inclination();
		// 		if (angle < 0)
		// 			angle += 360;
		// 		angle = angle - angle % 45;

		// 		shared_claw1->box.h = CPLB_B_ATTACK_H;
		// 		shared_claw1->box.w = CPLB_B_ATTACK_W;

		// 		shared_claw2->box.h = CPLB_B_ATTACK_H + CPLB_B_ATTACK_W / 2;
		// 		shared_claw2->box.w = CPLB_B_ATTACK_W;

		// 		// Verify which direction Capelobo is facing to create the hitbox on the right position
		// 		switch (dir)
		// 		{
		// 		case RIGHT:
		// 			shared_claw1->box.Centered(associated.box.Center()+Vec2({CPLB_HB_DISTANCE_X,0}));
		// 			break;

		// 		case UP:
		// 			std::swap(shared_claw1->box.h, shared_claw1->box.w);
		// 			shared_claw1->box.Centered(associated.box.Center()-Vec2({0,CPLB_HB_DISTANCE_X}));
		// 			break;

		// 		case LEFT:
		// 			shared_claw1->box.Centered(associated.box.Center()-Vec2({CPLB_HB_DISTANCE_X,0}));
		// 			break;

		// 		case DOWN:
		// 			std::swap(shared_claw1->box.h, shared_claw1->box.w);
		// 			shared_claw1->box.Centered(associated.box.Center()+Vec2({0,CPLB_HB_DISTANCE_X}));
		// 			break;

		// 		case LEFT_DOWN:
		// 			std::swap(shared_claw2->box.h, shared_claw2->box.w);
		// 			shared_claw2->box.Centered(associated.box.Center()+Vec2({-shared_claw1->box.w/4,CPLB_HB_DISTANCE_Y}));
					
		// 			theClaw2 = new Claw(*shared_claw2, CLAW_DAMAGE,true);
		// 			shared_claw2->AddComponent(theClaw2);

		// 			shared_claw1->box.Centered(associated.box.Center()-Vec2({CPLB_HB_DISTANCE_X,0}));
		// 			shared_claw1->box.h -= shared_claw2->box.h/2;
		// 			break;

		// 		case LEFT_UP:
		// 			std::swap(shared_claw2->box.h, shared_claw2->box.w);
		// 			shared_claw2->box.Centered(associated.box.Center()-Vec2({shared_claw1->box.w/4,CPLB_HB_DISTANCE_Y}));
					
		// 			theClaw2 = new Claw(*shared_claw2, CLAW_DAMAGE,true);
		// 			shared_claw2->AddComponent(theClaw2);

		// 			shared_claw1->box.Centered(associated.box.Center()-Vec2({CPLB_HB_DISTANCE_X,-shared_claw2->box.h/2}));
		// 			shared_claw1->box.h -= shared_claw2->box.h/2;
		// 			break;

		// 		case RIGHT_DOWN:
		// 			std::swap(shared_claw2->box.h, shared_claw2->box.w);
		// 			shared_claw2->box.Centered(associated.box.Center()+Vec2({shared_claw1->box.w/4,CPLB_HB_DISTANCE_Y}));
					
		// 			theClaw2 = new Claw(*shared_claw2, CLAW_DAMAGE,true);
		// 			shared_claw2->AddComponent(theClaw2);
					
		// 			shared_claw1->box.Centered(associated.box.Center()+Vec2({CPLB_HB_DISTANCE_X,0}));
		// 			shared_claw1->box.h -= shared_claw2->box.h/2;
		// 			break;

		// 		case RIGHT_UP:
		// 			std::swap(shared_claw2->box.h, shared_claw2->box.w);
		// 			shared_claw2->box.Centered(associated.box.Center()+Vec2({shared_claw1->box.w/4,-CPLB_HB_DISTANCE_Y}));
					
		// 			theClaw2 = new Claw(*shared_claw2, CLAW_DAMAGE,true);
		// 			shared_claw2->AddComponent(theClaw2);

		// 			shared_claw1->box.Centered(associated.box.Center()+Vec2({CPLB_HB_DISTANCE_X,shared_claw2->box.h/2}));
		// 			shared_claw1->box.h -= shared_claw2->box.h/2;
		// 			break;
		// 		default:
		// 			break;
		// 		}

		// 		theClaw1 = new Claw(*shared_claw1, CLAW_DAMAGE, true);

		// 		shared_claw1->AddComponent(theClaw1);
		// 	}

		// 	// End attack
		// 	if (attackTimer.Get() > 1)
		// 	{
		// 		state = RESTING;
		// 		attackTimer.Restart();
		// 		restTimer.Restart();
		// 	}
		// 	break;
		}
	}
}

void Dark_Spirit::Render() {
    Vec2 position = associated.box.Center();

	Sprite *sp = static_cast<Sprite *>(associated.GetComponent("Sprite"));
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
