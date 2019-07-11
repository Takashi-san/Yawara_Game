#include "Camera.h"
#include "InputManager.h"
#include "Game.h"

std::weak_ptr<GameObject> Camera::focus1;
std::weak_ptr<GameObject> Camera::focus2;
Vec2 Camera::pos;
Vec2 Camera::speed;
float Camera::ratio;

void Camera::Follow(std::weak_ptr<GameObject> focusN1) {
	focus1 = focusN1;
}

void Camera::Follow(std::weak_ptr<GameObject> focusN1, std::weak_ptr<GameObject> focusN2) {
	focus1 = focusN1;
	focus2 = focusN2;
}

void Camera::Unfollow() {
	focus1 = std::weak_ptr<GameObject>();
	focus2 = std::weak_ptr<GameObject>();
}

void Camera::Update(float dt) {
	std::shared_ptr<GameObject> focusA = focus1.lock();
	std::shared_ptr<GameObject> focusB = focus2.lock();

	if ((focusA != nullptr) && (focusB != nullptr)) {
		pos = {(focusA->box.Center() - focusB->box.Center()).Modulo() * ratio, 0};
		pos.Rotate(atan2((focusB->box.Center() - focusA->box.Center()).y, (focusB->box.Center() - focusA->box.Center()).x));
		pos.x = pos.x + focusA->box.Center().x - Game::GetInstance().GetRenderSize().x/2 /* + Game::GetInstance().GetRenderSize().x/2*/;
		pos.y = pos.y + focusA->box.Center().y - Game::GetInstance().GetRenderSize().y/2 /*+ Game::GetInstance().GetRenderSize().y/2*/;
	} else if (focusA != nullptr) {
		pos.x = focusA->box.Center().x - Game::GetInstance().GetRenderSize().x/2 /*+ Game::GetInstance().GetRenderSize().x/2*/;
		pos.y = focusA->box.Center().y - Game::GetInstance().GetRenderSize().y/2 /*+ Game::GetInstance().GetRenderSize().y/2*/;
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