#include "PauseState.h"
#include "StageState.h"
#include "Game.h"
#include "CameraFollower.h"
#include "InputManager.h"
#include "Camera.h"
#include "Text.h"
#include "Timer.h"

#include "Fonts.h"

#define PAUSE_STT_BG "assets/img/background/main_menu.png"

PauseState::PauseState()
{
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;

	// Pause img.
	GameObject *go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	ptr->box.x = Camera::pos.x;
	ptr->box.y = Camera::pos.y;
	Sprite *sp = new Sprite(*ptr, PAUSE_STT_BG);
	ptr->AddComponent(sp);

	// Pause txt.
	GameObject *t1go = new GameObject();
	weak_ptr = AddObject(t1go);
	ptr = weak_ptr.lock();
	Text *tx1 = new Text(*ptr, TTF_TEMPSITC, 50, Text::BLENDED, "Resume", {255, 255, 255, 255});
	ptr->box.Centered({341 + Camera::pos.x, 500 + Camera::pos.y});
	ptr->AddComponent(tx1);

	GameObject *t2go = new GameObject();
	weak_ptr = AddObject(t2go);
	ptr = weak_ptr.lock();
	Text *tx2 = new Text(*ptr, TTF_TEMPSITC, 50, Text::BLENDED, "Quit", {255, 255, 255, 255});
	ptr->box.Centered({683 + Camera::pos.x, 500 + Camera::pos.y});
	ptr->AddComponent(tx2);

	// Selection.
	GameObject *sgo = new GameObject();
	weak_ptr = AddObject(sgo);
	ptr = weak_ptr.lock();
	selection = sgo;
	Text *txs = new Text(*ptr, TTF_TEMPSITC, 50, Text::BLENDED, ">>", {255, 255, 255, 255}, 0.3);
	ptr->box.Centered({240 + Camera::pos.x, 500 + Camera::pos.y});
	ptr->AddComponent(txs);

	// BGM

	opt = RESUME;
}

PauseState::~PauseState()
{
	objectArray.clear();
}

void PauseState::Update(float dt)
{
	InputManager &input = InputManager::GetInstance();

	if(input.KeyPress(ESCAPE_KEY)){
		popRequested = true;
	}

	if(input.QuitRequested()){
		quitRequested = true;
	}

	if (input.KeyPress(A_KEY) || input.KeyPress(LEFT_ARROW_KEY))
	{
		opt = RESUME;
		selection->box.Centered({240 + Camera::pos.x, 500 + Camera::pos.y});
	}
	if (input.KeyPress(D_KEY) || input.KeyPress(RIGHT_ARROW_KEY))
	{
		opt = QUIT;
		selection->box.Centered({600 + Camera::pos.x, 500 + Camera::pos.y});
	}

	if (input.KeyPress(ENTER_KEY) || input.KeyPress(ENTER_KEY2))
	{
		switch (opt)
		{
		case RESUME:
			popRequested = true;
			break;

		case QUIT:
			quitRequested = true;
			break;

		default:
			break;
		}
	}

	UpdateArray(dt);
}

void PauseState::Render()
{
	RenderArray();
}

void PauseState::LoadAssets()
{
}

void PauseState::Start()
{
	LoadAssets();

	StartArray();

	started = true;
}

void PauseState::Pause()
{
}

void PauseState::Resume()
{
	Camera::pos.x = 0;
	Camera::pos.y = 0;
}