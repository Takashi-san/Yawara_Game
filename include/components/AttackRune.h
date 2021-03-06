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

    enum Image {INIMIGO = 0, YAWARA, ENCONTRO, AJUDA};

    Image top_img;
    AttackRune(GameObject&, float, Image = INIMIGO);
    
    void Update(float);
    void Render();
    bool Is(std::string);

    void Start();
};