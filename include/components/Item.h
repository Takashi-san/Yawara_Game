#pragma once

#include "Component.h"

#include <string>

#define RUNE_SOUND_PATH "assets/audio/sons/interacao_runa.ogg"

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