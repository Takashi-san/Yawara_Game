#pragma once

#include "Item.h"
#include "Sprite.h"
#include "Sound.h"

#include <string>

class GameObject;

class HealthRune : public Item
{
private:
    float hpFactor;
    Sprite* sp, *top_layer_sprite;
    Sound* activationSound;

public:
    HealthRune(GameObject&, float);
    
    void Update(float);
    void Render();
    bool Is(std::string);

    void Start();
};