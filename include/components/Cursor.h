#pragma once // Alows to initializate the header just once

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class Cursor : public Component
{
private:
public:
	Cursor(GameObject &);

	void Update(float);
	void Render();
	bool Is(std::string);
};