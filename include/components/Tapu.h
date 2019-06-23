#pragma once // Alows to initializate the header just once

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class Tapu : public Component
{
    private:
        std::weak_ptr<GameObject> yawara;
        float angle;
        int radius;

        enum Direction {LEFT, RIGHT};
        Direction dir;
        bool changedDir;

        float damageFactor;

    public:
        Tapu(GameObject &, std::weak_ptr<GameObject>);

        void Update(float);
        void Render();
        bool Is(std::string);
        void NotifyCollision(GameObject &);

        void Shoot();

        void SetDamageFactor(float);
};