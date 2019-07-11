#include "CameraFollower.h"
#include "Camera.h"
#include "Game.h"

CameraFollower::CameraFollower(GameObject& associated): Component(associated){

}

void CameraFollower::Update(float dt) {
	associated.box.Centered(Camera::pos + Game::GetInstance().GetWindowSize()/2);	
}

void CameraFollower::Render() {

}

bool CameraFollower::Is(std::string type) {
	return !strcmp(type.c_str(), "CameraFollower");
}