#include "Howl.h"
#include "Yawara.h"
#include "Easing.h"

#define HOWL_SOUND	"assets/audio/sons/yawara/yawara_howl.ogg"

#define HOWL_SPRITE	"assets/img/yawara/yawara_howl.png"
#define HOWL_FRAME	7

#define HOWL_MIN_ALPHA	0
#define HOWL_MAX_ALPHA	178

#define HOWL_MIN_SCALE	0.2
#define HOWL_MAX_SCALE	0.7

Howl::Howl(GameObject& associated) : Component(associated) {
	sound = new Sound(associated, HOWL_SOUND);
	associated.AddComponent(sound);

	sec2Activate = 0;
	activeTime = 0;
	exec = false;
}

void Howl::Start() {
	Sprite* sp = new Sprite(associated, HOWL_SPRITE, HOWL_FRAME);
	associated.AddComponent(sp);
	sprite = sp;
	sprite->SetAlphaMod(0);
}

bool Howl::Is(std::string type) {
	return !strcmp(type.c_str(), "Howl");
}

void Howl::Render() {

}

void Howl::SetHowl(float sec2Activate, float activeTime) {
	this->sec2Activate = sec2Activate;
	this->activeTime = activeTime;
	sound->Play(1, 100);
}

void Howl::Update(float dt) {
	static float scale;

	if (sec2Activate != 0) {
		toActivate.Update(dt);
		if (toActivate.Get() > sec2Activate) {
			sec2Activate = 0;
			toActivate.Restart();

			associated.box.Centered(Yawara::player->GetCenterPos());
			sprite->SetFrame(0);
			sprite->SetFrameCount(HOWL_FRAME);
			sprite->SetFrameTime(activeTime / HOWL_FRAME);
			sprite->SetStopFrame(HOWL_FRAME - 1);


			exec = true;
		}
	}

	if (exec) {
		toActivate.Update(dt);

		// Scale easing.
		scale = HOWL_MIN_SCALE + ((HOWL_MAX_SCALE - HOWL_MIN_SCALE) * CubicEaseOut(toActivate.Get()/activeTime));
		sprite->SetScale(scale, scale);

		// Alpha easing.
		if (toActivate.Get() < 2*(activeTime/3)) {
			sprite->SetAlphaMod(HOWL_MIN_ALPHA + ((HOWL_MAX_ALPHA - HOWL_MIN_ALPHA) * CubicEaseOut( toActivate.Get() / (2*activeTime/3) )));
		} else {
			sprite->SetAlphaMod(HOWL_MIN_ALPHA + ((HOWL_MAX_ALPHA - HOWL_MIN_ALPHA) * (1 - CubicEaseIn( (toActivate.Get() - (2*activeTime/3)) / (activeTime/3) ))));
		}

		associated.box.Centered(Yawara::player->GetCenterPos());
		
		// Finish execution.
		if (toActivate.Get() > activeTime) {
			exec = false;
			activeTime = 0;
			toActivate.Restart();

			sprite->SetAlphaMod(0);
			sound->FadeOut(500);
		}
	}
}