#include "HealthFeedback.h"
#include "Yawara.h"
#include "Easing.h"

#define HF_SPRITE	"assets/img/yawara/healthfeedback_2.png"

#define HF_MID_RATIO	0.6	// %.
#define HF_LOW_RATIO	0.3

#define HF_EASE_CICLE	3	// segundos.

#define HF_MIN_ALPHA	70	// 0 ~ 255.
#define HF_MAX_ALPHA	118	// Considerar os valores de HF_MOD_ALPHA e o seu mod mais baixo.
#define HF_MOD_ALPHA	9

#define HF_MIN_SCALE	0.7	// %.
#define HF_MAX_SCALE	0.9

#define HF_CLR_EXTRA	35,184,144	// R,G,B.
#define HF_CLR_FULL		255,187,166
#define HF_CLR_CAUTION	255,136,101
#define HF_CLR_DANGER	184,43,0
#define HF_CLR_DEAD		0,0,0

#define HF_EXTRA_MOD	1.3	// %.
#define HF_FULL_MOD		1.0
#define HF_CAUTION_MOD	0.5
#define HF_DANGER_MOD	0.1
#define HF_DEAD_MOD		1.0

HealthFeedback::HealthFeedback(GameObject& associated) : Component(associated) {
	cicle = true;
}

void HealthFeedback::Start() {
	Sprite* sp = new Sprite(associated, HF_SPRITE);
	associated.AddComponent(sp);
	sprite = sp;
	sprite->SetBlendMode(BLEND_ADD);
	sprite->SetAlphaMod(HF_MIN_ALPHA);
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
			sprite->SetColorMod(HF_CLR_EXTRA);
			mod = HF_EXTRA_MOD;
		} else if (hp > max_hp * HF_MID_RATIO) {
			sprite->SetColorMod(HF_CLR_FULL);
			mod = HF_FULL_MOD;
		} else if (hp > max_hp * HF_LOW_RATIO) {
			sprite->SetColorMod(HF_CLR_CAUTION);
			mod = HF_CAUTION_MOD;
		} else if (hp > 0) {
			sprite->SetColorMod(HF_CLR_DANGER);
			mod = HF_DANGER_MOD;
		} else if (hp <= 0) {
			sprite->SetColorMod(HF_CLR_DEAD);
			mod = HF_DEAD_MOD;
		}

		if (ease_time.Get() > HF_EASE_CICLE * mod) {
			ease_time.Restart();
			cicle = !cicle;
		}

		if (cicle) {
			sprite->SetAlphaMod(HF_MIN_ALPHA + ((HF_MAX_ALPHA - HF_MIN_ALPHA + HF_MOD_ALPHA/mod) * CubicEaseInOut(ease_time.Get()/(HF_EASE_CICLE * mod))));
			scale = HF_MIN_SCALE + ((HF_MAX_SCALE - HF_MIN_SCALE) * CubicEaseInOut(ease_time.Get()/(HF_EASE_CICLE * mod)));
			sprite->SetScale(scale, scale);
		} else {
			sprite->SetAlphaMod(HF_MIN_ALPHA + ((HF_MAX_ALPHA - HF_MIN_ALPHA + HF_MOD_ALPHA/mod) * (1 - CubicEaseInOut(ease_time.Get()/(HF_EASE_CICLE * mod)))));
			scale = HF_MIN_SCALE + ((HF_MAX_SCALE - HF_MIN_SCALE) * (1 - CubicEaseInOut(ease_time.Get()/(HF_EASE_CICLE * mod))));
			sprite->SetScale(scale, scale);
		}
	}
}