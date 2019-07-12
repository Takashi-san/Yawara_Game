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
#include "Easing.h"


#define CPLB_HB_DISTANCE_Y 150
#define CPLB_HB_DISTANCE_X 50

#define CPLB_B_ATTACK_H 400
#define CPLB_B_ATTACK_W 300

#define CPLB_L_ATTACK_H 20
#define CPLB_L_ATTACK_W 20

#define CPLB_ENEMY_DIST_Y 150
#define CPLB_ENEMY_DIST_X 150

#define BOSS_VEL_ANG 0
#define BOSS_SPEED 350
#define BOSS_REST_BASE 1.5
#define BOSS_MOVEMENT 1.5

#define CLAW_DAMAGE 10
#define TONGUE_DAMAGE 50
#define TONGUE_SPEED 1300
#define TONGUE_MAX_DIST 400

#define DIST_DETECT_YAWARA 700
#define DIST_LOAD_ATTACK 450
#define DIST_OUT_OF_RANGE 900

#define ATTACK_SOUND_DELAY 0.5

#define CPLB_BREATH 1.3

// Movement sprites

const std::string MOVE_RIGHT 	  = "assets/img/capelobo/capelobo_correndo_r.png";
const std::string MOVE_RIGHT_DOWN = "assets/img/capelobo/capelobo_correndo_rd.png";
const std::string MOVE_DOWN		  = "assets/img/capelobo/capelobo_correndo_ld.png";
const std::string MOVE_LEFT_DOWN  = "assets/img/capelobo/capelobo_correndo_ld.png";
const std::string MOVE_LEFT		  = "assets/img/capelobo/capelobo_correndo_l.png";
const std::string MOVE_LEFT_UP	  = "assets/img/capelobo/capelobo_correndo_lu.png";
const std::string MOVE_UP		  = "assets/img/capelobo/capelobo_correndo_r.png";
const std::string MOVE_RIGHT_UP	  = "assets/img/capelobo/capelobo_correndo_ru.png";

// Resting sprites

const std::string REST_RIGHT		= "assets/img/capelobo/capelobo_idle_right.png";
const std::string REST_RIGHT_DOWN 	= "assets/img/capelobo/capelobo_idle_right_down.png";
const std::string REST_DOWN		 	= "assets/img/capelobo/capelobo_idle_down.png";
const std::string REST_LEFT_DOWN	= "assets/img/capelobo/capelobo_idle_left_down.png";
const std::string REST_LEFT		 	= "assets/img/capelobo/capelobo_idle_left.png";
const std::string REST_LEFT_UP	 	= "assets/img/capelobo/capelobo_idle_left_up.png";
const std::string REST_UP		 	= "assets/img/capelobo/capelobo_idle_up.png";
const std::string REST_RIGHT_UP		= "assets/img/capelobo/capelobo_idle_right_up.png";

// Attacking sprites

const std::string ATTACK_RIGHT		= "assets/img/capelobo/capelobo_attack_r.png";
const std::string ATTACK_RIGHT_DOWN	= "assets/img/capelobo/capelobo_attack_r.png";
const std::string ATTACK_DOWN		= "assets/img/capelobo/capelobo_attack_d.png";
const std::string ATTACK_LEFT_DOWN	= "assets/img/capelobo/capelobo_attack_l.png";
const std::string ATTACK_LEFT		= "assets/img/capelobo/capelobo_attack_l.png";
const std::string ATTACK_LEFT_UP	= "assets/img/capelobo/capelobo_attack_l.png";
const std::string ATTACK_UP			= "assets/img/capelobo/capelobo_attack_u.png";
const std::string ATTACK_RIGHT_UP	= "assets/img/capelobo/capelobo_attack_r.png";

// Load attack sprites

const std::string LOAD_RIGHT		= "assets/img/capelobo/capelobo_ataque_lingua_r.png";
const std::string LOAD_RIGHT_DOWN 	= "";
const std::string LOAD_DOWN		 	= "assets/img/capelobo/capelobo_lingua_d.png";
const std::string LOAD_LEFT_DOWN	= "assets/img/capelobo/capelobo_ataque_lingua_ld.png";
const std::string LOAD_LEFT		 	= "assets/img/capelobo/capelobo_ataque_lingua_l.png";
const std::string LOAD_LEFT_UP	 	= "";
const std::string LOAD_UP			= "assets/img/capelobo/capelobo_lingua_u.png";
const std::string LOAD_RIGHT_UP	 	= "";

// Death sprites

const std::string DEATH_RIGHT		= "assets/img/capelobo/capelobo_morte_r.png";
const std::string DEATH_LEFT	 	= "assets/img/capelobo/capelobo_morte_l.png";
const std::string EFFECT_1			= "assets/img/capelobo/camada1_efeitomorteboss.png";
const std::string EFFECT_2			= "assets/img/capelobo/camada2_efeitomorteboss.png";
const std::string EFFECT_3			= "assets/img/capelobo/camada3_efeitomorteboss.png";
const std::string EFFECT_4			= "assets/img/capelobo/camada4_efeitomorteboss.png";
const std::string EFFECT_5			= "assets/img/capelobo/camada5_efeitomorteboss.png";

// Sounds

const std::string CPLB_HIT_SOUND	= "assets/audio/sons/capelobo/hit.ogg";
const std::string CPLB_B_ATK_SOUND1	= "assets/audio/sons/capelobo/ataque1.ogg";
const std::string CPLB_B_ATK_SOUND2	= "assets/audio/sons/capelobo/ataque2.ogg";
const std::string CPLB_L_ATK_SOUND	= "assets/audio/sons/capelobo/LINGUA VAI.ogg";
const std::string BREATH_SOUND		= "assets/audio/sons/capelobo/respira.ogg";
const std::string CPLB_EXPLODE		= "assets/audio/sons/capelobo/explosao.ogg";
const std::string CPLB_DEATH		= "assets/audio/sons/capelobo/grunhido_morte.ogg";
const std::string SAW_YAWARA		= "assets/audio/sons/capelobo/ve_inimigo.ogg";

Capelobo *Capelobo::boss;

bool cplbStartedAttack = false;
bool cplbStartedMoving = true;
bool cplbSpriteChanged = false;
bool endEffects = false;

Vec2 temp_speed;

Capelobo::Capelobo(GameObject &associated, std::weak_ptr<GameObject> weak_shadow, float restOffset) : Enemy(associated)
{
	boss = this;
	moveAllowed = true;
	timesPlayed = 0;

	Sprite *sp = new Sprite(associated, REST_RIGHT, 8, 0.150);
	Collider *cl = new Collider(associated);
	associated.AddComponent(sp);
	associated.AddComponent(cl);

	speed = Vec2{0,0};

	state = SLEEPING;
	hp = 60;
	this->restOffset = restOffset;
	this->weak_shadow = weak_shadow;
}

void Capelobo::Start()
{
	// Reset all timers
	restTimer.Restart();
	moveTimer.Restart();
	attackTimer.Restart();
	hitTimer.Restart();
	soundTimer.Restart();
	deathTimer.Restart();
}

Capelobo::~Capelobo()
{
	// Capelobo is dead
	boss = nullptr;
}

void Capelobo::Update(float dt)
{
	associated.angleDeg += (BOSS_VEL_ANG / (PI / 180)) * dt;
	std::shared_ptr<GameObject> shadow = weak_shadow.lock();
	shadow->box.Centered(associated.box.Center());

	GameObject *go = new GameObject();
	std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
	std::shared_ptr<GameObject> ptr = weak_ptr.lock();
	Sound *so = new Sound(*ptr, BREATH_SOUND);

	hitTimer.Update(dt);
	soundTimer.Update(dt);

	if (Yawara::player != nullptr && hp > 0)
	{
		yawaraPos = Yawara::player->GetCenterPos();
		switch (state)
		{
		case PURSUE:
			moveTimer.Update(dt);

			if ((yawaraPos - associated.box.Center()).Modulo() < DIST_OUT_OF_RANGE && moveAllowed)
			{

				if(moveTimer.Get() > BOSS_MOVEMENT && (yawaraPos - associated.box.Center()).Modulo() <= DIST_LOAD_ATTACK)
				{
					// Stop moving.
					speed.x = 0;
					speed.y = 0;

					// Change to LOAD_ATTACK state.
					cplbStartedAttack = false;
					cplbSpriteChanged = false;
					state = LOAD_ATTACK;
					cplbStartedMoving = false;
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
					cplbStartedAttack = false;
					cplbSpriteChanged = false;
					state = BASIC_ATTACK;
					timesPlayed = 0;
					velAttackOffset = 1;
					soundOffset = 0;
					cplbStartedMoving = false;
					break;
				}

				// Correct root to move in 45 degrees
				Move45(speed);

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

				if (dir != lastDir || cplbStartedMoving){
					change_sprite = true;
					cplbStartedMoving = false;
				}

				moveAllowed = AllowedToMove(speed);
				
				if (!moveAllowed) // If he can't move, he will rest
				{
					state = RESTING;
					restTimer.Restart();
					cplbSpriteChanged = true;
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
				cplbStartedMoving = false;
			}
			break;

		case RESTING:{

			restTimer.Update(dt);
			if (!cplbSpriteChanged){
				change_sprite = true;
				cplbSpriteChanged = true;
			}
			else change_sprite = false;

			// Rest for a determinated time
			if (restTimer.Get() > BOSS_REST_BASE + restOffset)
			{
				if ((yawaraPos - associated.box.Center()).Modulo() != 0)
					speed = ((yawaraPos - associated.box.Center()) / ((yawaraPos - associated.box.Center()).Modulo())) * BOSS_SPEED;
				else
					speed = {0, 0};

				temp_speed = speed;

				state = PURSUE;
				cplbStartedMoving = true;
				moveAllowed = 1;
				moveTimer.Restart();
				so->Open(SAW_YAWARA);
				so->Play(1);

			}
			if(soundTimer.Get() > CPLB_BREATH && (yawaraPos - associated.box.Center()).Modulo() < 1000){
				so->Play(1);
				soundTimer.Restart();
			}
			break;
		}

		case SLEEPING:{
			// Stay asleep if Yawara don't get closer
			if ((yawaraPos - associated.box.Center()).Modulo() <= DIST_DETECT_YAWARA){
				state = PURSUE;
				moveTimer.Restart();
				cplbStartedMoving = true;
				so->Open(SAW_YAWARA);
				so->Play(1);
				break;
			}
			if(soundTimer.Get() > CPLB_BREATH && (yawaraPos - associated.box.Center()).Modulo() < 1000){
				so->Open(BREATH_SOUND);
				so->Play(1);
				soundTimer.Restart();
			}
			break;
		}
		case BASIC_ATTACK:
			attackTimer.Update(dt);

			// Make change sprite just when the attack start and if it hasn't changed yet
			if (change_sprite && !cplbStartedAttack){
				change_sprite = false;
				cplbSpriteChanged = false;
			}

			if (!change_sprite && !cplbSpriteChanged){
				change_sprite = true;
				cplbSpriteChanged = true;
			}

			if(dir == UP || dir == DOWN){
				velAttackOffset = 2;
				soundOffset = 0.45;
			}

			// Create Hitbox if it wasn't created yet. Create it 0.3s after render the sprite (time that he actualy attack on the sprite)
			if (attackTimer.Get() > 0.3 * velAttackOffset)
			{
				GameObject *soundGO = new GameObject();
				std::weak_ptr<GameObject> weak_claw = Game::GetInstance().GetCurrentState().AddObject(soundGO);
				std::shared_ptr<GameObject> shared_claw = weak_claw.lock();
				Sound *so = new Sound(*shared_claw, CPLB_B_ATK_SOUND1);
				shared_claw->AddComponent(so);
				
				// Create first Claw hitbox
				GameObject *clawGO = new GameObject();
				weak_claw = Game::GetInstance().GetCurrentState().AddObject(clawGO);
				shared_claw = weak_claw.lock();
				Claw *theClaw;

				if(timesPlayed < 2 && soundTimer.Get() > ATTACK_SOUND_DELAY - soundOffset){
					so->Play(1, MIX_MAX_VOLUME);
					so->Open(CPLB_B_ATK_SOUND2);
					++timesPlayed;
					soundTimer.Restart();
				}

				std::weak_ptr<GameObject> weak_Boss = Game::GetInstance().GetCurrentState().GetObjectPtr(&associated);

				int angle = (yawaraPos - associated.box.Center()).Inclination();
				if (angle < 0)
					angle += 360;
				angle = angle - angle % 45;

				shared_claw->box.h = CPLB_B_ATTACK_H;
				shared_claw->box.w = CPLB_B_ATTACK_W;

				// Verify which direction Capelobo is facing to create the hitbox on the right position
				switch (dir)
				{
				case RIGHT:
					shared_claw->box.Centered(associated.box.Center()+Vec2({CPLB_HB_DISTANCE_X,0}));
					break;

				case UP:
				case RIGHT_UP:
				case LEFT_UP:
					std::swap(shared_claw->box.h, shared_claw->box.w);
					shared_claw->box.Centered(associated.box.Center()-Vec2({0,CPLB_HB_DISTANCE_X}));
					break;

				case LEFT:
					shared_claw->box.Centered(associated.box.Center()-Vec2({CPLB_HB_DISTANCE_X,0}));
					break;

				case DOWN:
				case RIGHT_DOWN:
				case LEFT_DOWN:
					std::swap(shared_claw->box.h, shared_claw->box.w);
					shared_claw->box.Centered(associated.box.Center()+Vec2({0,CPLB_HB_DISTANCE_X}));
					break;

				default:
					break;
				}

				theClaw = new Claw(*shared_claw, CLAW_DAMAGE, true);

				shared_claw->AddComponent(theClaw);
			}

			// End attack
			if (attackTimer.Get() > 0.9)
			{
				state = RESTING;
				attackTimer.Restart();
				restTimer.Restart();
				cplbSpriteChanged = false;
			}
			break;
		
		case LOAD_ATTACK:
			
			attackTimer.Update(dt);


			// Make change sprite just when the attack start and if it hasn't changed yet
			if (change_sprite && !cplbStartedAttack){
				change_sprite = false;
				cplbSpriteChanged = false;
			}

			if (!change_sprite && !cplbSpriteChanged){
				change_sprite = true;
				cplbSpriteChanged = true;
			}


			// Create Hitbox if it wasn't created yet. Create it 0.3s after render the sprite (time that he actualy attack on the sprite)
			if (!cplbStartedAttack)
			{
				GameObject *soundGO = new GameObject();
				std::weak_ptr<GameObject> weak_claw = Game::GetInstance().GetCurrentState().AddObject(soundGO);
				std::shared_ptr<GameObject> shared_claw = weak_claw.lock();
				Sound *so = new Sound(*shared_claw, CPLB_L_ATK_SOUND);
				shared_claw->AddComponent(so);

				GameObject *tongueGO = new GameObject();
				std::weak_ptr<GameObject> weak_tongue = Game::GetInstance().GetCurrentState().AddObject(tongueGO);
				std::shared_ptr<GameObject> shared_tongue = weak_tongue.lock();

				shared_tongue->box.w = CPLB_L_ATTACK_W;
				shared_tongue->box.h = CPLB_L_ATTACK_H;
				shared_tongue->box.x = associated.box.Center().x - shared_tongue->box.w / 2;
				shared_tongue->box.y = associated.box.Center().y;

				int angle = (yawaraPos - associated.box.Center()).Inclination();
				if (angle < 0)
					angle += 360;

				so->Play(1, MIX_MAX_VOLUME);

				cplbStartedAttack = true;

				// Corect direction and angle. Capelobo will attack diagonally just if it is on a range od 15 deg from the 45 deg diagonal
				if(angle > 330 || angle < 60){
					angle = 360;
					dir = RIGHT;
					distOffset = 0;
				}
				else if(angle > 60 && angle < 150){
					angle = 90;
					dir = DOWN;
					distOffset = 100;
				}
				else if(angle > 150 && angle < 240){
					angle = 180;
					dir = LEFT;
					distOffset = 0;
				}
				else{
					angle = 270;
					dir = UP;
					distOffset = 100;
				}

				Tongue *theTongue = new Tongue(*shared_tongue, TONGUE_DAMAGE, TONGUE_SPEED, angle, TONGUE_MAX_DIST - distOffset, true);

				shared_tongue->AddComponent(theTongue);
				if(shared_tongue != nullptr)
					change_sprite = true;
			}

			// End attack
			if (attackTimer.Get() > 0.6)
			{
				state = PURSUE;
				cplbStartedMoving = true;
				attackTimer.Restart();
				restTimer.Restart();
				so->Open(SAW_YAWARA);
				so->Play(1);
			}
			break;
		default:
			state = RESTING;
			restTimer.Restart();
			break;
		}
	}

	// Capelobo is dead
	if (hp <= 0)
	{
		change_sprite = true;
		state = RESTING;
		deathTimer.Update(dt);
		
		Sprite *sp;

		if(deathTimer.Get() < DEATH_EFFECT_TIME){
			if(deathTimer.Get() < DEATH_EFFECT_TIME / 10){
				sp = new Sprite(*ptr, EFFECT_1, 1, DEATH_EFFECT_TIME / 10, DEATH_EFFECT_TIME / 10);
				timesPlayed = 0;
			}
			else if (deathTimer.Get() < (DEATH_EFFECT_TIME * 2) / 10)
				sp = new Sprite(*ptr, EFFECT_2, 1, DEATH_EFFECT_TIME / 10, DEATH_EFFECT_TIME / 10);
			else if (deathTimer.Get() < (DEATH_EFFECT_TIME * 3) / 10){
				sp = new Sprite(*ptr, EFFECT_4, 1, DEATH_EFFECT_TIME / 10, DEATH_EFFECT_TIME / 10);
				sp->SetScale(Vec2{0.5,0.5});
			}
			else if(deathTimer.Get() < (DEATH_EFFECT_TIME * 4) / 10)
				sp = new Sprite(*ptr, EFFECT_4, 1, DEATH_EFFECT_TIME / 10, DEATH_EFFECT_TIME / 10);
			else if(deathTimer.Get() < (DEATH_EFFECT_TIME * 5) / 10){
				sp = new Sprite(*ptr, EFFECT_4, 1, DEATH_EFFECT_TIME / 10, DEATH_EFFECT_TIME / 10);
				sp->SetColorMod(150,30,140);
			}
			else{
				so->Open(CPLB_EXPLODE);
				sp = new Sprite(*ptr, EFFECT_5, 7, DEATH_EFFECT_TIME /10, 7 * (DEATH_EFFECT_TIME /10));
				sp->SetColorMod(150,30,140);
				sp->SetAlphaMod(200);
				// sp->SetBlendMode(BLEND_ADD);
				if(timesPlayed == 0){
					so->Play(1, MIX_MAX_VOLUME);
					++timesPlayed;
				}
			}
		}
		else{
			shadow->RequestDelete();
			associated.RequestDelete();

			if(dir == RIGHT || dir == RIGHT_DOWN || dir == RIGHT_UP || dir == UP)
				sp = new Sprite(*ptr, DEATH_RIGHT, 9, 0.1, 9 * 0.1);
			else
				sp = new Sprite(*ptr, DEATH_LEFT, 9, 0.1, 9 * 0.1);
			so->Open(CPLB_DEATH);
			ptr->AddComponent(so);

			so->Play(1);
		}
		ptr->box.Centered(associated.box.Center());
		ptr->AddComponent(sp);
	}
}

void Capelobo::Render()
{
	Vec2 position = associated.box.Center();

	Sprite *sp = static_cast<Sprite *>(associated.GetComponent("Sprite"));
	if (change_sprite && sp)
	{
		change_sprite = false;
		if (state == PURSUE)
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
				sp->SetFrameCount(5);
				break;

			case DOWN:
				sp->Open(REST_DOWN);
				sp->SetFrameCount(5);
				break;

			case RIGHT_UP:
				sp->Open(REST_RIGHT_UP);
				sp->SetFrameCount(5);
				break;

			case RIGHT_DOWN:
				sp->Open(REST_RIGHT_DOWN);
				sp->SetFrameCount(5);
				break;

			case LEFT_UP:
				sp->Open(REST_LEFT_UP);
				sp->SetFrameCount(5);
				break;

			case LEFT_DOWN:
				sp->Open(REST_LEFT_DOWN);
				sp->SetFrameCount(5);
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
				sp->SetFrameTime(0.15);
				sp->Open(ATTACK_UP);
				sp->SetFrameCount(5);
				break;

			case DOWN:
				sp->SetFrameTime(0.15);
				sp->Open(ATTACK_DOWN);
				sp->SetFrameCount(5);
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
				sp->Open(LOAD_UP);
				sp->SetFrameCount(6);
				break;

			case DOWN:
				sp->Open(LOAD_DOWN);
				sp->SetFrameCount(6);
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
		else if(state == SLEEPING){
			// sp->SetFrameTime(0.08);
			// sp->Open(LOAD_LEFT_DOWN);
			// sp->SetFrameCount(4);
		}
	}
	associated.box.Centered(position);
}

bool Capelobo::Is(std::string type)
{
	return !std::min(strcmp(type.c_str(), "Capelobo"),strcmp(type.c_str(), "Enemy"));
}

void Capelobo::HitSound(){
    GameObject *soundGO = new GameObject();
    std::weak_ptr<GameObject> weak_hit = Game::GetInstance().GetCurrentState().AddObject(soundGO);
    std::shared_ptr<GameObject> shared_hit = weak_hit.lock();
    Sound *so = new Sound(*shared_hit, CPLB_HIT_SOUND);
    shared_hit->AddComponent(so);
    so->Play(1,MIX_MAX_VOLUME);
}