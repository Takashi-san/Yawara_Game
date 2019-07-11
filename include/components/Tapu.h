#pragma once // Alows to initializate the header just once

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"
#include "Sprite.h"
#include "Sound.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class Tapu : public Component
{
    private:
        std::weak_ptr<GameObject> yawara;
        float angle, height;

        enum Direction {LEFT, RIGHT, UP, DOWN, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT};
        Direction dir;
        bool changedDir;

        std::weak_ptr<GameObject> shadow_ptr;

        float damageFactor;

        Sound* shoot;
    public:
        Tapu(GameObject &, std::weak_ptr<GameObject>);

        void Update(float);
        void Start();
        void Render();
        bool Is(std::string);
        void NotifyCollision(GameObject &);

        void Shoot();

        void SetDamageFactor(float);
};