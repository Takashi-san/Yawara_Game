#pragma once

#include "Item.h"

#include <string>

class GameObject;

class HealthRune : public Item
{
private:

    float hpFactor;
public:
    HealthRune(GameObject&, float);
    
    void Update(float);
    void Render();
    bool Is(std::string);

    void Start();
};