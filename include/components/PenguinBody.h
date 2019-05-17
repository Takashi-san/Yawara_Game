#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

#ifndef PENGUIN_BODY
	#define PENGUIN_BODY

	#define PBODY_ACCEL 500
	#define PBODY_VEL_CAP 300
	#define PBODY_VEL_ANG 2

	class PenguinBody : public Component{
	private:
		std::weak_ptr<GameObject> pcannon;
		Vec2 speed;
		float linearSpeed;
		float angle;
		int hp;

	public:
		static PenguinBody* player;
		
		PenguinBody(GameObject&);
		~PenguinBody();

		void Start();
		void Update(float);
		void Render();
		bool Is(std::string);
		void NotifyCollision(GameObject&);

		Vec2 GetPos();
	};
#endif