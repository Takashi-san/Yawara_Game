#include "Cursor.h"
#include "InputManager.h"
#include "Camera.h"
#include "MapColision.h"

Cursor::Cursor(GameObject &associated) : Component(associated)
{
}

void Cursor::Update(float dt)
{
	InputManager &input = InputManager::GetInstance();

	associated.box.Centered({(float)input.GetMouseX() + Camera::pos.x, (float)input.GetMouseY() + Camera::pos.y});

	std::cout << "validation: " << MapColision::GetInstance().Validate(associated.box) << "\n";
	//MapColision::GetInstance().Validate(associated.box.x, associated.box.y);
}

void Cursor::Render()
{
}

bool Cursor::Is(std::string type)
{
	return !strcmp(type.c_str(), "Cursor");
}
