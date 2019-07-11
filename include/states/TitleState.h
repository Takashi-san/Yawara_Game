#pragma once // Alows to initializate the header just once

#include "State.h"
#include "Music.h"
#include "Sprite.h"
#include "Sound.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class TitleState : public State
{
private:
	GameObject* selection;
	std::shared_ptr<GameObject> controlsgo;
	enum Option { PLAY, OPTIONS, QUIT };
	Option opt;
	Music bgMusic;
	Sprite* selectionSprite;
	Sound *changeSelection, *play, *select;

	float xpos[3], ypos;

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