#include "State.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

#ifndef TITLESTATE
	#define TITLESTATE

	class TitleState : public State{
	public:
		TitleState();
		~TitleState();

		void LoadAssets();
		void Update(float dt);
		void Render();

		void Start();
		void Pause();
		void Resume();
	};

#endif