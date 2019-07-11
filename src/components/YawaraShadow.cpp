#include "YawaraShadow.h"
#include "Yawara.h"

#define YWR_SHADOW_LR			"assets/img/yawara/yawara_shadow.png"
#define YWR_SHADOW_D			"assets/img/yawara/yawara_shadow_down.png"
#define YWR_SHADOW_U			"assets/img/yawara/yawara_shadow_up.png"
#define YWR_SHADOW_DL			"assets/img/yawara/yawara_shadow_down_left.png"
#define YWR_SHADOW_DR			"assets/img/yawara/yawara_shadow_down_right.png"
#define YWR_SHADOW_UL			"assets/img/yawara/yawara_shadow_up_left.png"
#define YWR_SHADOW_UR			"assets/img/yawara/yawara_shadow_up_right.png"

YawaraShadow::YawaraShadow(GameObject& associated) : Component(associated) {
}

void YawaraShadow::Start() {
	Sprite* sp = new Sprite(associated, YWR_SHADOW_LR);
	associated.AddComponent(sp);
	sprite = sp;
}

void YawaraShadow::Render() {
	if (Yawara::player) {
		dir = (Direction)(Yawara::player->GetDirection());
		if (sprite) {
				switch (dir) {
					case RIGHT:
						sprite->Open(YWR_SHADOW_LR);
					break;

					case LEFT:
						sprite->Open(YWR_SHADOW_LR);
					break;

					case UP:
						sprite->Open(YWR_SHADOW_U);
					break;

					case DOWN:
						sprite->Open(YWR_SHADOW_D);
					break;

					case UP_RIGHT:
						sprite->Open(YWR_SHADOW_UR);
					break;

					case DOWN_RIGHT:
						sprite->Open(YWR_SHADOW_DR);
					break;

					case UP_LEFT:
						sprite->Open(YWR_SHADOW_UL);
					break;

					case DOWN_LEFT:
						sprite->Open(YWR_SHADOW_DL);
					break;

					default:
					break;
				}
			}
		associated.box.Centered(Yawara::player->GetCenterPos());	// Under the player.
	}
}

bool YawaraShadow::Is(std::string type) {
	return !strcmp(type.c_str(), "YawaraShadow");
}

void YawaraShadow::Update(float dt) {
}