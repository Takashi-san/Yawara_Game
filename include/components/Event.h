#pragma once

#include "Component.h"

class Event : public Component
{
private:
    
public:
    Event(GameObject&, float, float, float, float);

    void Update(float);
	void Render();
	bool Is(std::string);
	void NotifyCollision(GameObject &);

	void Start();
};
