#include "TitleState.h"
#include "StageState.h"
#include "Game.h"
#include "CameraFollower.h"
#include "InputManager.h"
#include "Camera.h"
#include "Text.h"
#include "Timer.h"

TitleState::TitleState() {
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;

	// Title img.
	GameObject *go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	ptr->box.x = 0;
	ptr->box.y = 0;
	Sprite *sp = new Sprite(*ptr, "assets/img/title.jpg");
	CameraFollower *cmfl = new CameraFollower(*ptr);
	ptr->AddComponent(sp);
	ptr->AddComponent(cmfl);

	// Title txt.
	GameObject *tgo = new GameObject();
	weak_ptr = AddObject(tgo);
	ptr = weak_ptr.lock();
	Text* tx = new Text(*ptr, "assets/font/Call me maybe.ttf", 50, Text::BLENDED, "Press Space To Start!", {255, 0, 0, 255}, 0.5, {0, 0, 255, 255});
	ptr->box.Centered({512, 450});
	ptr->AddComponent(tx);
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

	// Inicia o jogo.
	if (input.KeyPress(SPACE_KEY)) { 
		StageState *stage = new StageState();
		Game::GetInstance().Push(stage);
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

	started = true;
}

void TitleState::Pause() {

}

void TitleState::Resume() {
	Camera::pos.x = 0;
	Camera::pos.y = 0;
}