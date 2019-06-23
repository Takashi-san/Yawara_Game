#pragma once

#include "Item.h"
#include "Sprite.h"

#include <string>

class GameObject;

class DefenseRune : public Item
{
private:
    float defFactor;
    Sprite* sp;
    
public:
    DefenseRune(GameObject&, float);
    
    void Update(float);
    void Render();
    bool Is(std::string);

    void Start();
};