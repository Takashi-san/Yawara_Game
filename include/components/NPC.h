#pragma once

#include "Component.h"

class NPC : public Component
{
    private:
        bool  interactive;

        void Action();
        
    public:
        NPC(GameObject, bool = false);
        
        void Update(float);
};
