#pragma once

#include "Item.h"
#include "Sprite.h"
#include "Sound.h"

#include <string>

class GameObject;

class AttackRune : public Item
{
private:
    float attFactor;
    Sprite* sp, *top_layer_sprite;
    Sound* activationSound;
    
public:

    enum Color {DARK = 0, MEDIUM, LIGHT};
    enum Image {YAWARA = 0, TAPU};

    Image top_img;
    AttackRune(GameObject&, float, Color = DARK, Image = YAWARA);
    
    void Update(float);
    void Render();
    bool Is(std::string);

    void Start();

    void ChangeColor(Color);
};