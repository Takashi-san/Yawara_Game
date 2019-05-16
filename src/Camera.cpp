#include "Camera.h"
#include "InputManager.h"

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
		// Se mudar o tamanho da tela tem que mudar aqui!!!
		pos.x = focus->box.Center().x - 1024/2;
		pos.y = focus->box.Center().y - 600/2;
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