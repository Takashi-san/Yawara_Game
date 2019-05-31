#pragma once // Alows to initializate the header just once

#include "State.h"
#include "Music.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class TitleState : public State
{
private:
	GameObject* selection;
	enum Option { PLAY, QUIT };
	Option opt;

public:

	TitleState();
	~TitleState();

		void Start();
		void Pause();
		void Resume();

	private:
		Music bgMusic;
	};

	void Start();
	void Pause();
	void Resume();
};