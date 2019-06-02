#pragma once // Alows to initializate the header just once

#include "State.h"
#include "Music.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class EndState : public State
{
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