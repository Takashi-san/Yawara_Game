#pragma once

#include "Component.h"

#include <string>

class GameObject;

class Item : public Component
{
private:

public:
    Item(GameObject&);

    void Update(float);
    void Render();
    bool Is(std::string);

    void Start();
};