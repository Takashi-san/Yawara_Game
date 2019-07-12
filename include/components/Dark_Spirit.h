#pragma once // Alows to initializate the header just once

#include "Enemy.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>
#include <vector>
#include <queue>
#include <cmath>

class Dark_Spirit : public Enemy
{
	private:
		float restOffset;
	public:
		Dark_Spirit(GameObject &);
		~Dark_Spirit();

		void Update(float) override;
		void Render() override;
		bool Is(std::string);
		void HitSound() override;
};