#include "Camera.h"
#include "InputManager.h"
#include "Game.h"

GameObject* Camera::focus;
Vec2 Camera::pos;
Vec2 Camera::speed;

void Camera::Follow(GameObject* newFocus) {
	focus = newFocus;
}

void Camera::Unfollow() {
	focus = nullptr;
}

void Camera::Update(float dt) {
	if (focus != nullptr) {
		pos.x = focus->box.Center().x - Game::GetInstance().GetWindowSize().x/2;
		pos.y = focus->box.Center().y - Game::GetInstance().GetWindowSize().y/2;
	} else {
		InputManager& input = InputManager::GetInstance();

		if (input.IsKeyDown(RIGHT_ARROW_KEY)) {
			speed.x = SPEEDC;
		} else if (input.IsKeyDown(LEFT_ARROW_KEY)) {
			speed.x = -SPEEDC;
		} else {
			speed.x = 0;
		}

		if (input.IsKeyDown(UP_ARROW_KEY)) {
			speed.y = -SPEEDC;
		} else if (input.IsKeyDown(DOWN_ARROW_KEY)) {
			speed.y = SPEEDC;
		} else {
			speed.y = 0;
		}

		pos.x += speed.x*dt;
		pos.y += speed.y*dt;
	}
}