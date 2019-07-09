#include "HealthFeedback.h"
#include "Yawara.h"
#include "Easing.h"

#define FB_SPRITE	"assets/img/yawara/healthfeedback_2.png"

#define MID_RATIO	0.6	// %.
#define LOW_RATIO	0.3

#define EASE_CICLE	3	// segundos.

#define MIN_ALPHA	70	// 0 ~ 255.
#define MAX_ALPHA	118	// Considerar os valores de MOD_ALPHA e o seu mod mais baixo.
#define MOD_ALPHA	9

#define MIN_SCALE	0.7	// %.
#define MAX_SCALE	0.9

#define CLR_EXTRA	35,184,144	// R,G,B.
#define CLR_FULL	255,187,166
#define CLR_CAUTION	255,136,101
#define CLR_DANGER	184,43,0
#define CLR_DEAD	0,0,0

#define EXTRA_MOD	1.3	// %.
#define FULL_MOD	1.0
#define CAUTION_MOD	0.5
#define DANGER_MOD	0.1
#define DEAD_MOD	1.0

HealthFeedback::HealthFeedback(GameObject& associated) : Component(associated) {
	cicle = true;
}

void HealthFeedback::Start() {
	Sprite* sp = new Sprite(associated, FB_SPRITE);
	associated.AddComponent(sp);
	sprite = sp;
	sprite->SetBlendMode(BLEND_ADD);
	sprite->SetAlphaMod(MIN_ALPHA);
}

void HealthFeedback::Render() {
	if (Yawara::player) {
		associated.box.Centered(Yawara::player->GetCenterPos());	// Under the player.
	}
}

bool HealthFeedback::Is(std::string type) {
	return !strcmp(type.c_str(), "HealthFeedback");
}

void HealthFeedback::Update(float dt) {
	static int hp, max_hp;
	static float scale, mod;

	if (Yawara::player) {
		hp = Yawara::player->GetHP();
		max_hp = Yawara::player->GetMaxHP();
		associated.box.Centered(Yawara::player->GetCenterPos());	// On top of the player;
	}

	ease_time.Update(dt);

	if (sprite) {
		if (hp > max_hp) {
			sprite->SetColorMod(CLR_EXTRA);
			mod = EXTRA_MOD;
		} else if (hp > max_hp * MID_RATIO) {
			sprite->SetColorMod(CLR_FULL);
			mod = FULL_MOD;
		} else if (hp > max_hp * LOW_RATIO) {
			sprite->SetColorMod(CLR_CAUTION);
			mod = CAUTION_MOD;
		} else if (hp > 0) {
			sprite->SetColorMod(CLR_DANGER);
			mod = DANGER_MOD;
		} else if (hp <= 0) {
			sprite->SetColorMod(CLR_DEAD);
			mod = DEAD_MOD;
		}

		if (ease_time.Get() > EASE_CICLE * mod) {
			ease_time.Restart();
			cicle = !cicle;
		}

		if (cicle) {
			sprite->SetAlphaMod(MIN_ALPHA + ((MAX_ALPHA - MIN_ALPHA + MOD_ALPHA/mod) * CubicEaseInOut(ease_time.Get()/(EASE_CICLE * mod))));
			scale = MIN_SCALE + ((MAX_SCALE - MIN_SCALE) * CubicEaseInOut(ease_time.Get()/(EASE_CICLE * mod)));
			sprite->SetScale(scale, scale);
		} else {
			sprite->SetAlphaMod(MIN_ALPHA + ((MAX_ALPHA - MIN_ALPHA + MOD_ALPHA/mod) * (1 - CubicEaseInOut(ease_time.Get()/(EASE_CICLE * mod)))));
			scale = MIN_SCALE + ((MAX_SCALE - MIN_SCALE) * (1 - CubicEaseInOut(ease_time.Get()/(EASE_CICLE * mod))));
			sprite->SetScale(scale, scale);
		}
	}
}