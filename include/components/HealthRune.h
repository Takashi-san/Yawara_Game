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
    Sprite* sp, *top_layer_sprite, *ressonance;
    Sound* activationSound;

public:
    enum Color {DARK = 0, MEDIUM, LIGHT};

    HealthRune(GameObject&, float, Color = DARK);
    
    void Update(float);
    void Render();
    bool Is(std::string);

    void Start();

    void ChangeColor(Color);
};