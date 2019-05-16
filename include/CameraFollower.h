#include "GameObject.h"
#include "Component.h"

#include <string>
#include <string.h>
#include <iostream>
#include <stdbool.h>

#ifndef CAMERA_FOLLOW
	#define CAMERA_FOLLOW

	class CameraFollower: public Component{
	public:
		CameraFollower(GameObject&);

		void Update(float);
		void Render();
		bool Is(std::string);
	};

#endif