#include "Component.h"
#include "GameObject.h"
#include "Vec2.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

#ifndef CURSOR
	#define CURSOR

	class Cursor : public Component{
	private:

	public:
		Cursor(GameObject&);

		void Update(float);
		void Render();
		bool Is(std::string);
	};
#endif