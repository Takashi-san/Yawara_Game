#include "Alien.h"
#include "Sprite.h"
#include "InputManager.h"
#include "Camera.h"
#include "State.h"
#include "Game.h"
#include "Minion.h"
#include "Collider.h"
#include "Bullet.h"
#include "Sound.h"
#include "PenguinBody.h"

int Alien::alienCount;

Alien::Alien(GameObject& associated, int nMinions, float restOffset) : Component(associated) {
	hp = 70;
	speed.x = 0;
	speed.y = 0;
	this->nMinions = nMinions;
	this->restOffset = restOffset;
	Sprite *sp = new Sprite(associated, "assets/img/alien.png");
	Collider *cl = new Collider(associated, {0.85, 0.85});
	associated.AddComponent(sp);
	associated.AddComponent(cl);

	alienCount++;
	state = RESTING;
}

void Alien::Start() {
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;
	float offset;

	for (int i = 0; i < nMinions; i++){
		offset = (360/nMinions)*i;

		GameObject *go = new GameObject();
		weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
		minionArray.emplace_back(weak_ptr);
		ptr = weak_ptr.lock();
		
		Minion *mini = new Minion(*ptr, Game::GetInstance().GetCurrentState().GetObjectPtr(&associated), offset);
		ptr->box.x = 0;
		ptr->box.y = 0;
		ptr->AddComponent(mini);
	}

}

Alien::~Alien() {
	minionArray.clear();

	alienCount--;
}

void Alien::Update(float dt) {
	float dst = 999999;

	associated.angleDeg += (ALIEN_VEL_ANG/0.0174533)*dt;

	if (PenguinBody::player != nullptr) {
		int j;
		switch (state) {
			case MOVING:
				// Se chegou no destino.
				if ((associated.box.Center().x <= destination.x + dt*abs(speed.x)) && (associated.box.Center().x >= destination.x - dt*abs(speed.x)) &&
					(associated.box.Center().y <= destination.y + dt*abs(speed.y)) && (associated.box.Center().y >= destination.y - dt*abs(speed.y))) {
					// Coloca na posição final.
					associated.box.Centered(destination);
					speed.x = 0;
					speed.y = 0;

					// Comanda o minion atirar.
					destination = PenguinBody::player->GetPos();
					for (unsigned int i = 0; i < minionArray.size(); i++) {
						std::shared_ptr<GameObject> ptr = minionArray[i].lock();
						if (ptr) {
							float tmp = (destination - ptr->box.Center()).Modulo();
							if (tmp < dst) {
								dst = tmp;
								j = i;
							}
						}
					}
					if (minionArray[j].lock()){
						Minion *mini = static_cast<Minion*>(minionArray[j].lock()->GetComponent("Minion"));
						mini->Shoot(destination);
					}

					// Muda estado.
					state = RESTING;
					restTimer.Restart();
					break;
				}

				// Anda.
				associated.box.x += speed.x*dt;
				associated.box.y += speed.y*dt;
			break;

			case RESTING:
				restTimer.Update(dt);
				if (restTimer.Get() > ALIEN_REST_BASE + restOffset) {
					destination = PenguinBody::player->GetPos();
					if ((destination - associated.box.Center()).Modulo() != 0) {
						speed = ((destination - associated.box.Center())/((destination - associated.box.Center()).Modulo())) * ALIEN_SPEED;
					} else {
						speed = {0, 0};
					}

					state = MOVING;
				}
			break;
		}
	}

	if (hp <= 0) {
		associated.RequestDelete();

		GameObject *go = new GameObject();
		std::weak_ptr<GameObject> weak_ptr = Game::GetInstance().GetCurrentState().AddObject(go);
		std::shared_ptr<GameObject> ptr = weak_ptr.lock();
		
		Sprite* sp = new Sprite(*ptr, "assets/img/aliendeath.png", 4, 0.1, 4*0.1);
		Sound *so = new Sound(*ptr, "assets/audio/boom.wav");
		ptr->box.Centered(associated.box.Center());
		ptr->AddComponent(sp);
		ptr->AddComponent(so);

		so->Play(1);
	}
}

void Alien::Render() {

}

bool Alien::Is(std::string type) {
	return !strcmp(type.c_str(), "Alien");
}

void Alien::NotifyCollision(GameObject& other) {
	if (other.GetComponent("Bullet") && !static_cast<Bullet*>(other.GetComponent("Bullet"))->targetsPlayer) {
		hp -= static_cast<Bullet*>(other.GetComponent("Bullet"))->GetDamage();
	}
}