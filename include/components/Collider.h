#pragma once // Alows to initializate the header just once

#include "Vec2.h"
#include "GameObject.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class Collider : public Component
{
private:
	Vec2 scale;
	Vec2 offset;

public:
	Rect box;

	Collider(GameObject &, Vec2 = {1, 1}, Vec2 = {0, 0});

	void Update(float);
	void Render();
	bool Is(std::string);

	void SetScale(Vec2);
	void SetOffset(Vec2);
	void SetDimension();
	void SetDimension(Vec2);
	void SetDimension(float, float);
};