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

class Capelobo : public Enemy
{
private:
	
	float restOffset;
	float velAttackOffset; // Correct time of attack execution due to some sprites of attack are with 5 sprites and othes 16
	float soundOffset;
	float distOffset;
	std::weak_ptr<GameObject> weak_shadow;

public:
	static Capelobo *boss;

	Capelobo(GameObject &, std::weak_ptr<GameObject>, float = 0);
	~Capelobo();

	void Start() override;
	void Update(float) override;
	void Render() override;
	bool Is(std::string);
	void HitSound() override;
};