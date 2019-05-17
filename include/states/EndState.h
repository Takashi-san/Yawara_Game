#include "State.h"
#include "Music.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

#ifndef ENDSTATE
	#define ENDSTATE

	class EndState : public State{
	public:
		EndState();
		~EndState();

		void LoadAssets();
		void Update(float dt);
		void Render();

		void Start();
		void Pause();
		void Resume();

	private:
		Music bgMusic;
	};

#endif