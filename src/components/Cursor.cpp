#include "Cursor.h"
#include "InputManager.h"
#include "Camera.h"
#include "Floor.h"

Cursor::Cursor(GameObject& associated) : Component(associated) {
	
}

void Cursor::Update(float dt) {
	InputManager& input = InputManager::GetInstance();

	associated.box.Centered({(float)input.GetMouseX() + Camera::pos.x, (float)input.GetMouseY() + Camera::pos.y});

	Floor *theFloor = static_cast<Floor *>(associated.GetComponent("Floor"));
	std::cout << theFloor->AtAllowedArea(associated.box.Center().x, associated.box.Center().y, 0) << "\n";
}

void Cursor::Render() {

}

bool Cursor::Is(std::string type) {
	return !strcmp(type.c_str(), "Cursor");
}
