#include "TitleState.h"
#include "StageState.h"
#include "Game.h"
#include "CameraFollower.h"
#include "InputManager.h"
#include "Camera.h"
#include "Text.h"
#include "Timer.h"

TitleState::TitleState() : bgMusic("assets/audio/menu.mp3"){
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;

	// Title img.
	GameObject *go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	ptr->box.x = 0;
	ptr->box.y = 0;
	Sprite *sp = new Sprite(*ptr, "assets/img/menu.png");
	CameraFollower *cmfl = new CameraFollower(*ptr);
	ptr->AddComponent(sp);
	ptr->AddComponent(cmfl);

	// Title txt.
	GameObject *t1go = new GameObject();
	weak_ptr = AddObject(t1go);
	ptr = weak_ptr.lock();
	Text* tx1 = new Text(*ptr, "assets/font/tempsitc.ttf", 50, Text::BLENDED, "Play", {255, 255, 255, 255});
	ptr->box.Centered({341, 500});
	ptr->AddComponent(tx1);

	GameObject *t2go = new GameObject();
	weak_ptr = AddObject(t2go);
	ptr = weak_ptr.lock();
	Text* tx2 = new Text(*ptr, "assets/font/tempsitc.ttf", 50, Text::BLENDED, "Quit", {255, 255, 255, 255});
	ptr->box.Centered({683, 500});
	ptr->AddComponent(tx2);

	// Selection.
	GameObject *sgo = new GameObject();
	weak_ptr = AddObject(sgo);
	ptr = weak_ptr.lock();
	selection = sgo;
	Text* txs = new Text(*ptr, "assets/font/tempsitc.ttf", 50, Text::BLENDED, ">>", {255, 255, 255, 255}, 0.3);
	ptr->box.Centered({280, 500});
	ptr->AddComponent(txs);

	opt = PLAY;
}

TitleState::~TitleState() {
	objectArray.clear();
}

void TitleState::Update(float dt) {
	InputManager& input = InputManager::GetInstance();

	// Fecha o jogo.
	if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
		quitRequested = true;
	}

	if (input.KeyPress(A_KEY) || input.KeyPress(LEFT_ARROW_KEY)) {
		opt = PLAY;
		selection->box.Centered({260, 500});
	}
	if (input.KeyPress(D_KEY) || input.KeyPress(RIGHT_ARROW_KEY)) {
		opt = QUIT;
		selection->box.Centered({600, 500});
	}

	if (input.KeyPress(ENTER_KEY)) {
		StageState *stage = new StageState();
		switch (opt) {
			case PLAY:
				Game::GetInstance().Push(stage);
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

void TitleState::Render() {
	RenderArray();
}

void TitleState::LoadAssets() {

}

void TitleState::Start(){
	LoadAssets();
	
	StartArray();

	bgMusic.Play();

	started = true;
}

void TitleState::Pause() {

}

void TitleState::Resume() {
	Camera::pos.x = 0;
	Camera::pos.y = 0;
}