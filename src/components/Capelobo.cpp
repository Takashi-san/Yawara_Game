#include "Capelobo.h"
#include "Sprite.h"
#include "InputManager.h"
#include "Camera.h"
#include "State.h"
#include "Game.h"
#include "Collider.h"
#include "Bullet.h"
#include "Sound.h"
#include "Yawara.h"
#include "Claw.h"
#include "Tongue.h"

#define CPLB_HB_DISTANCE_Y 150
#define CPLB_HB_DISTANCE_X 150

#define CPLB_B_ATTACK_H 300
#define CPLB_B_ATTACK_W 90

#define CPLB_L_ATTACK_H 20
#define CPLB_L_ATTACK_W 20

#define CPLB_ENEMY_DIST_Y 150
#define CPLB_ENEMY_DIST_X 150

#define BOSS_VEL_ANG 0
#define BOSS_SPEED 350
#define BOSS_REST_BASE 0.5
#define BOSS_MOVEMENT 1.5

#define CLAW_DAMAGE 10
#define TONGUE_DAMAGE 50
#define TONGUE_SPEED 1300
#define TONGUE_MAX_DIST 400

#define DIST_DETECT_YAWARA 700
#define DIST_LOAD_ATTACK 450

// Movement sprites

const std::string MOVE_RIGHT 	  = "assets/img/capelobo/capelobo_correndo_line_art_r.png";
const std::string MOVE_RIGHT_DOWN = "assets/img/capelobo/capelobo_correndo_line_art_rd.png";
const std::string MOVE_DOWN		  = "assets/img/capelobo/capelobo_correndo_line_art_l.png";
const std::string MOVE_LEFT_DOWN  = "assets/img/capelobo/capelobo_correndo_line_art_ld.png";
const std::string MOVE_LEFT		  = "assets/img/capelobo/capelobo_correndo_line_art_l.png";
const std::string MOVE_LEFT_UP	  = "assets/img/capelobo/capelobo_correndo_line_art_lu.png";
const std::string MOVE_UP		  = "assets/img/capelobo/capelobo_correndo_line_art_r.png";
const std::string MOVE_RIGHT_UP	  = "assets/img/capelobo/capelobo_correndo_line_art_ru.png";

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

// Load attack sprites

const std::string LOAD_RIGHT		= "assets/img/capelobo/capelobo_ataque_lingua_r.png";
const std::string LOAD_RIGHT_DOWN 	= "";
const std::string LOAD_DOWN		 	= "";
const std::string LOAD_LEFT_DOWN	= "assets/img/capelobo/capelobo_ataque_lingua_ld.png";
const std::string LOAD_LEFT		 	= "assets/img/capelobo/capelobo_ataque_lingua_l.png";
const std::string LOAD_LEFT_UP	 	= "";
const std::string LOAD_UP			= "";
const std::string LOAD_RIGHT_UP	 	= "";

Capelobo *Capelobo::boss;

bool moveAllowed = true;
bool startedAttack = false;
bool startedMoving = true;
bool changed = false;

Vec2 temp_speed;

Capelobo::Capelobo(GameObject &associated, float restOffset) : Enemy(associated)
{
	boss = this;

	hp = 600;
	speed.x = 0;
	speed.y = 0;
	this->restOffset = restOffset;
	Sprite *sp = new Sprite(associated, "assets/img/capelobo/capelobo_idle_left.png", 8, 0.150);
	Collider *cl = new Collider(associated);
	associated.AddComponent(sp);
	associated.AddComponent(cl);

	state = SLEEPING;
}

void Capelobo::Start()
{
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;
	
	// Reset all timers
	restTimer.Restart();
	moveTimer.Restart();
	attackTimer.Restart();
	hitTimer.Restart();
}

Capelobo::~Capelobo()
{
	// Capelobo is dead
	boss = nullptr;
}

void Capelobo::Update(float dt)
{
	associated.angleDeg += (BOSS_VEL_ANG / (PI / 180)) * dt;

	hitTimer.Update(dt);

	if (Yawara::player != nullptr)
	{
		yawaraPos = Yawara::player->GetCenterPos();
		switch (state)
		{
		case MOVING:
			moveTimer.Update(dt);

			if ((yawaraPos - associated.box.Center()).Modulo() < 900 && moveAllowed)
			{

				if(moveTimer.Get() > BOSS_MOVEMENT && (yawaraPos - associated.box.Center()).Modulo() <= DIST_LOAD_ATTACK)
				{
					// Stop moving.
					speed.x = 0;
					speed.y = 0;

					// Change to LOAD_ATTACK state.
					startedAttack = false;
					changed = false;
					state = LOAD_ATTACK;
					restTimer.Restart();
					moveTimer.Restart();
				}

				if ((yawaraPos - associated.box.Center()).Modulo() != 0)
					speed = ((yawaraPos - associated.box.Center()) / ((yawaraPos - associated.box.Center()).Modulo())) * BOSS_SPEED;
				else
					speed = {0, 0};

				// Yawara is nearby the Capelobo.
				if (((associated.box.Center().x <= CPLB_ENEMY_DIST_X + yawaraPos.x + dt * abs(speed.x)) && (associated.box.Center().x >= -CPLB_ENEMY_DIST_X + yawaraPos.x - dt * abs(speed.x)) &&
					 (associated.box.Center().y <= CPLB_ENEMY_DIST_Y + yawaraPos.y + dt * abs(speed.y)) && (associated.box.Center().y >= -CPLB_ENEMY_DIST_Y + yawaraPos.y - dt * abs(speed.y))) ||
					!moveAllowed)
				{
					// Stop moving
					speed.x = 0;
					speed.y = 0;

					// Change to BASIC_ATTACK state.
					startedAttack = false;
					changed = false;
					state = BASIC_ATTACK;
					break;
				}

				// Correct root to move in 45 degrees
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

				// Set dir to the direction that Capelobo is facing

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
				if (dir != lastDir || startedMoving){
					change_sprite = true;
					startedMoving = false;
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
			else if((yawaraPos - associated.box.Center()).Modulo() > DIST_DETECT_YAWARA){
				state = SLEEPING;
			}
			break;

		case RESTING:
			restTimer.Update(dt);
			if (!change_sprite)
				change_sprite = true;

			// Rest for a determinated time
			if (restTimer.Get() > BOSS_REST_BASE + restOffset)
			{
				if ((yawaraPos - associated.box.Center()).Modulo() != 0)
					speed = ((yawaraPos - associated.box.Center()) / ((yawaraPos - associated.box.Center()).Modulo())) * BOSS_SPEED;
				else
					speed = {0, 0};

				temp_speed = speed;

				state = MOVING;
				startedMoving = true;
				moveAllowed = 1;
				moveTimer.Restart();
			}
			break;

		case SLEEPING:
			// Stay asleep if Yawara don't get closer
			if ((yawaraPos - associated.box.Center()).Modulo() <= DIST_DETECT_YAWARA)
				state = MOVING;
				startedMoving = true;

			break;

		case BASIC_ATTACK:
			attackTimer.Update(dt);

			// Make change sprite just when the attack start and if it hasn't changed yet
			if (change_sprite && !startedAttack){
				change_sprite = false;
				changed = false;
			}

			if (!change_sprite && !changed){
				change_sprite = true;
				changed = true;
			}

			// Create Hitbox if it wasn't created yet. Create it 0.3s after render the sprite (time that he actualy attack on the sprite)
			if (!startedAttack && attackTimer.Get() > 0.3)
			{
				// Create first Claw hitbox
				GameObject *clawGO = new GameObject();
				std::weak_ptr<GameObject> weak_claw1 = Game::GetInstance().GetCurrentState().AddObject(clawGO);
				std::shared_ptr<GameObject> shared_claw1 = weak_claw1.lock();
				Claw *theClaw1;

				// Create second Claw hitbox. It'll be used when diagonal attack
				GameObject *clawGO2 = new GameObject();
				std::weak_ptr<GameObject> weak_claw2 = Game::GetInstance().GetCurrentState().AddObject(clawGO2);
				std::shared_ptr<GameObject> shared_claw2 = weak_claw2.lock();
				Claw *theClaw2;

				startedAttack = true;

				std::weak_ptr<GameObject> weak_Boss = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);

				int angle = (Yawara::player->GetCenterPos() - associated.box.Center()).Inclination();
				if (angle < 0)
					angle += 360;
				angle = angle - angle % 45;

				shared_claw1->box.h = CPLB_B_ATTACK_H;
				shared_claw1->box.w = CPLB_B_ATTACK_W;

				shared_claw2->box.h = CPLB_B_ATTACK_H + CPLB_B_ATTACK_W / 2;
				shared_claw2->box.w = CPLB_B_ATTACK_W;

				// Verify which direction Capelobo is facing to create the hitbox on the right position
				switch (dir)
				{
				case RIGHT:
					shared_claw1->box.Centered(associated.box.Center()+Vec2({CPLB_HB_DISTANCE_X,0}));
					break;

				case UP:
					std::swap(shared_claw1->box.h, shared_claw1->box.w);
					shared_claw1->box.Centered(associated.box.Center()-Vec2({0,CPLB_HB_DISTANCE_X}));
					break;

				case LEFT:
					shared_claw1->box.Centered(associated.box.Center()-Vec2({CPLB_HB_DISTANCE_X,0}));
					break;

				case DOWN:
					std::swap(shared_claw1->box.h, shared_claw1->box.w);
					shared_claw1->box.Centered(associated.box.Center()+Vec2({0,CPLB_HB_DISTANCE_X}));
					break;

				case LEFT_DOWN:
					std::swap(shared_claw2->box.h, shared_claw2->box.w);
					shared_claw2->box.Centered(associated.box.Center()+Vec2({-shared_claw1->box.w/4,CPLB_HB_DISTANCE_Y}));
					
					theClaw2 = new Claw(*shared_claw2, CLAW_DAMAGE,true);
					shared_claw2->AddComponent(theClaw2);

					shared_claw1->box.Centered(associated.box.Center()-Vec2({CPLB_HB_DISTANCE_X,0}));
					shared_claw1->box.h -= shared_claw2->box.h/2;
					break;

				case LEFT_UP:
					std::swap(shared_claw2->box.h, shared_claw2->box.w);
					shared_claw2->box.Centered(associated.box.Center()-Vec2({shared_claw1->box.w/4,CPLB_HB_DISTANCE_Y}));
					
					theClaw2 = new Claw(*shared_claw2, CLAW_DAMAGE,true);
					shared_claw2->AddComponent(theClaw2);

					shared_claw1->box.Centered(associated.box.Center()-Vec2({CPLB_HB_DISTANCE_X,-shared_claw2->box.h/2}));
					shared_claw1->box.h -= shared_claw2->box.h/2;
					break;

				case RIGHT_DOWN:
					std::swap(shared_claw2->box.h, shared_claw2->box.w);
					shared_claw2->box.Centered(associated.box.Center()+Vec2({shared_claw1->box.w/4,CPLB_HB_DISTANCE_Y}));
					
					theClaw2 = new Claw(*shared_claw2, CLAW_DAMAGE,true);
					shared_claw2->AddComponent(theClaw2);
					
					shared_claw1->box.Centered(associated.box.Center()+Vec2({CPLB_HB_DISTANCE_X,0}));
					shared_claw1->box.h -= shared_claw2->box.h/2;
					break;

				case RIGHT_UP:
					std::swap(shared_claw2->box.h, shared_claw2->box.w);
					shared_claw2->box.Centered(associated.box.Center()+Vec2({shared_claw1->box.w/4,-CPLB_HB_DISTANCE_Y}));
					
					theClaw2 = new Claw(*shared_claw2, CLAW_DAMAGE,true);
					shared_claw2->AddComponent(theClaw2);

					shared_claw1->box.Centered(associated.box.Center()+Vec2({CPLB_HB_DISTANCE_X,shared_claw2->box.h/2}));
					shared_claw1->box.h -= shared_claw2->box.h/2;
					break;
				default:
					break;
				}

				theClaw1 = new Claw(*shared_claw1, CLAW_DAMAGE, true);

				shared_claw1->AddComponent(theClaw1);
			}

			// End attack
			if (attackTimer.Get() > 1)
			{
				state = RESTING;
				attackTimer.Restart();
				restTimer.Restart();
			}
			break;
		
		case LOAD_ATTACK:
			
			attackTimer.Update(dt);

			// Make change sprite just when the attack start and if it hasn't changed yet
			if (change_sprite && !startedAttack){
				change_sprite = false;
				changed = false;
			}

			if (!change_sprite && !changed){
				change_sprite = true;
				changed = true;
			}


			// Create Hitbox if it wasn't created yet. Create it 0.3s after render the sprite (time that he actualy attack on the sprite)
			if (!startedAttack)
			{
				startedAttack = true;

				GameObject *tongueGO = new GameObject();
				std::weak_ptr<GameObject> weak_tongue = Game::GetInstance().GetCurrentState().AddObject(tongueGO);
				std::shared_ptr<GameObject> shared_tongue = weak_tongue.lock();

				shared_tongue->box.w = CPLB_L_ATTACK_W;
				shared_tongue->box.h = CPLB_L_ATTACK_H;
				shared_tongue->box.x = associated.box.Center().x - shared_tongue->box.w / 2;
				shared_tongue->box.y = associated.box.Center().y;

				int angle = (Yawara::player->GetCenterPos() - associated.box.Center()).Inclination();
				if (angle < 0)
					angle += 360;


				// Corect direction and angle. Capelobo will attack diagonally just if it is on a range od 15 deg from the 45 deg diagonal
				if(angle > 330 || angle < 30){
					angle = 360;
					dir = RIGHT;
				}
				else if(angle > 30 && angle < 60){
					angle = 45;
					dir = RIGHT_UP;	
				}
				else if(angle > 60 && angle < 120){
					angle = 90;
					dir = UP;
				}
				else if(angle > 120 && angle < 150){
					angle = 135;
					dir = LEFT_UP;
				}
				else if(angle > 150 && angle < 210){
					angle = 180;
					dir = LEFT;
				}
				else if(angle > 210 && angle < 240){
					angle = 225;
					dir = LEFT_DOWN;
				}
				else if(angle > 240 && angle < 300){
					angle = 270;
					dir = DOWN;
				}
				else{
					angle = 315;
					dir = RIGHT_DOWN;
				}

				Tongue *theTongue = new Tongue(*shared_tongue, TONGUE_DAMAGE, TONGUE_SPEED, angle, TONGUE_MAX_DIST, true);

				shared_tongue->AddComponent(theTongue);
				if(shared_tongue != nullptr)
					change_sprite = true;
			}

			// End attack
			if (attackTimer.Get() > 0.6)
			{
				state = MOVING;
				startedMoving = true;
				attackTimer.Restart();
				restTimer.Restart();
			}
			break;
		}
	}

	// Capelobo is dead
	if (hp <= 0)
	{
		associated.RequestDelete();
		/*
		GameObject *go = new GameObject();
		std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
		std::shared_ptr<GameObject> ptr = weak_ptr.lock();

		Sprite *sp = new Sprite(*ptr, "assets/img/aliendeath.png", 4, 0.1, 4 * 0.1);
		Sound *so = new Sound(*ptr, "assets/audio/boom.wav");
		ptr->box.Centered(associated.box.Center());
		ptr->AddComponent(sp);
		ptr->AddComponent(so);

		so->Play(1);
		*/
	}
}

void Capelobo::Render()
{
	Vec2 position = associated.box.Center();

	Sprite *sp = static_cast<Sprite *>(associated.GetComponent("Sprite"));
	if (change_sprite && sp)
	{
		change_sprite = false;
		if (state == MOVING)
		{
			sp->SetFrameTime(0.1);
			switch (dir)
			{
			case RIGHT:
				sp->Open(MOVE_RIGHT);
				sp->SetFrameCount(12);
				break;

			case LEFT:
				sp->Open(MOVE_LEFT);
				sp->SetFrameCount(12);
				break;

			case UP:
				sp->Open(MOVE_UP);
				sp->SetFrameCount(12);
				break;

			case DOWN:
				sp->Open(MOVE_DOWN);
				sp->SetFrameCount(12);
				break;

			case RIGHT_UP:
				sp->Open(MOVE_RIGHT_UP);
				sp->SetFrameCount(12);
				break;

			case RIGHT_DOWN:
				sp->Open(MOVE_RIGHT_DOWN);
				sp->SetFrameCount(12);
				break;

			case LEFT_UP:
				sp->Open(MOVE_LEFT_UP);
				sp->SetFrameCount(12);
				break;

			case LEFT_DOWN:
				sp->Open(MOVE_LEFT_DOWN);
				sp->SetFrameCount(12);
				break;

			default:
				break;
			}
		}
		else if (state == RESTING)
		{
			sp->SetFrameTime(0.15);
			switch (dir)
			{
			case RIGHT:
				sp->Open(REST_RIGHT);
				sp->SetFrameCount(8);
				break;

			case LEFT:
				sp->Open(REST_LEFT);
				sp->SetFrameCount(8);
				break;

			case UP:
				sp->Open(REST_UP);
				sp->SetFrameCount(8);
				break;

			case DOWN:
				sp->Open(REST_DOWN);
				sp->SetFrameCount(8);
				break;

			case RIGHT_UP:
				sp->Open(REST_RIGHT_UP);
				sp->SetFrameCount(8);
				break;

			case RIGHT_DOWN:
				sp->Open(REST_RIGHT_DOWN);
				sp->SetFrameCount(8);
				break;

			case LEFT_UP:
				sp->Open(REST_LEFT_UP);
				sp->SetFrameCount(8);
				break;

			case LEFT_DOWN:
				sp->Open(REST_LEFT_DOWN);
				sp->SetFrameCount(8);
				break;

			default:
				break;
			}
		}
		else if (state == BASIC_ATTACK)
		{
			sp->SetFrameTime(0.05);
			switch (dir)
			{
			case RIGHT:
				sp->Open(ATTACK_RIGHT);
				sp->SetFrameCount(16);
				break;

			case LEFT:
				sp->Open(ATTACK_LEFT);
				sp->SetFrameCount(16);
				break;

			case UP:
				sp->Open(ATTACK_UP);
				sp->SetFrameCount(16);
				break;

			case DOWN:
				sp->Open(ATTACK_DOWN);
				sp->SetFrameCount(16);
				break;

			case RIGHT_UP:
				sp->Open(ATTACK_RIGHT_UP);
				sp->SetFrameCount(16);
				break;

			case RIGHT_DOWN:
				sp->Open(ATTACK_RIGHT_DOWN);
				sp->SetFrameCount(16);
				break;

			case LEFT_UP:
				sp->Open(ATTACK_LEFT_UP);
				sp->SetFrameCount(16);
				break;

			case LEFT_DOWN:
				sp->Open(ATTACK_LEFT_DOWN);
				sp->SetFrameCount(16);
				break;

			default:
				break;
			}
		}
		else if (state == LOAD_ATTACK)
		{
			sp->SetFrameTime(0.08);
			switch (dir)
			{
			case RIGHT:
				sp->Open(LOAD_RIGHT);
				sp->SetFrameCount(6);
				break;

			case LEFT:
				sp->Open(LOAD_LEFT);
				sp->SetFrameCount(6);
				break;

			case UP:
				// sp->Open(LOAD_UP);
				// sp->SetFrameCount(16);
				break;

			case DOWN:
				// sp->Open(LOAD_DOWN);
				// sp->SetFrameCount(16);
				break;

			case RIGHT_UP:
				// sp->Open(LOAD_RIGHT_UP);
				// sp->SetFrameCount(16);
				break;

			case RIGHT_DOWN:
				// sp->Open(LOAD_RIGHT_DOWN);
				// sp->SetFrameCount(16);
				break;

			case LEFT_UP:
				// sp->Open(LOAD_LEFT_UP);
				// sp->SetFrameCount(16);
				break;

			case LEFT_DOWN:
				sp->Open(LOAD_LEFT_DOWN);
				sp->SetFrameCount(4);
				break;

			default:
				break;
			}
		}
	}
	associated.box.Centered(position);
}

bool Capelobo::Is(std::string type)
{
	return !strcmp(type.c_str(), "Capelobo");
}