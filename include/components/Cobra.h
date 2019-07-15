#pragma once // Alows to initializate the header just once

#include "Enemy.h"
#include "GameObject.h"
#include "Vec2.h"
#include "State.h"
#include "Timer.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>
#include <vector>
#include <queue>
#include <cmath>

class Cobra : public Enemy
{
private:

public:
	static Cobra *boss;

	Cobra(GameObject &);
	~Cobra();

	void Start() override;
	void Update(float) override;
	void Render() override;
	bool Is(std::string);
	void HitSound() override;
};