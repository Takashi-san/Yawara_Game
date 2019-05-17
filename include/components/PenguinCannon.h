#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

#ifndef PENGUIN_CANNON
	#define PENGUIN_CANNON

	#define PCANNON_BULLET_SPEED 750
	#define PCANNON_BULLET_DAMAGE 10
	#define PCANNON_BULLET_RANGE 1000
	#define PCANNON_SHOOT_CD 0.4

	class PenguinCannon : public Component{
	private:
		std::weak_ptr<GameObject> pbody;
		float angle;

	public:
		PenguinCannon(GameObject&, std::weak_ptr<GameObject>);

		void Update(float);
		void Render();
		bool Is(std::string);
		void NotifyCollision(GameObject&);

		void Shoot();
	};
#endif