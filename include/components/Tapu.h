#pragma once // Alows to initializate the header just once

#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

#define TAPU_BULLET_SPEED 750
#define TAPU_BULLET_DAMAGE 10
#define TAPU_BULLET_RANGE 1000
#define TAPU_SHOOT_CD 0.4

class Tapu : public Component
{
    private:
        std::weak_ptr<GameObject> yawara;
        float angle;
        int radius;

        enum Direction {LEFT, RIGHT};
        Direction dir;
        bool changedDir;

    public:
        Tapu(GameObject &, std::weak_ptr<GameObject>);

        void Update(float);
        void Render();
        bool Is(std::string);
        void NotifyCollision(GameObject &);

        void Shoot();
};