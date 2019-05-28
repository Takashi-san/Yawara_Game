#pragma once // Alows to initializate the header just once

#include "GameObject.h"

#include <string>
#include <iostream>
#include <stdbool.h>

// Para evitar erro de deepnested.
class GameObject;

class Component
{
protected:
	GameObject &associated;

public:
	Component(GameObject &associated);
	virtual ~Component();

	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual bool Is(std::string type) = 0;
	virtual void NotifyCollision(GameObject &);

	virtual void Start();
};