#pragma once // Alows to initializate the header just once

#include "GameObject.h"
#include "Component.h"

#include <string>
#include <string.h>
#include <iostream>
#include <stdbool.h>

class CameraFollower : public Component
{
public:
	CameraFollower(GameObject &);

	void Update(float);
	void Render();
	bool Is(std::string);
};