#include "EndState.h"
#include "Data.h"
#include "Sprite.h"
#include "CameraFollower.h"
#include "Text.h"
#include "Camera.h"
#include "InputManager.h"

EndState::EndState() {
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;

	if (Data::playerVictory) {
		// End img.
		GameObject *go = new GameObject();
		weak_ptr = AddObject(go);
		ptr = weak_ptr.lock();
		ptr->box.x = 0;
		ptr->box.y = 0;
		Sprite *sp = new Sprite(*ptr, "assets/img/win.jpg");
		CameraFollower *cmfl = new CameraFollower(*ptr);
		ptr->AddComponent(sp);
		ptr->AddComponent(cmfl);

		// BGM
		bgMusic.Open("assets/audio/endStateWin.ogg");
	
		// End txt.
		GameObject *tgo = new GameObject();
		weak_ptr = AddObject(tgo);
		ptr = weak_ptr.lock();
		Text* tx = new Text(*ptr, "assets/font/Call me maybe.ttf", 50, Text::BLENDED, "Press Space To Play Again", {50, 50, 255, 255});
		ptr->box.Centered({512, 475});
		ptr->AddComponent(tx);

		GameObject *tgo1 = new GameObject();
		weak_ptr = AddObject(tgo1);
		ptr = weak_ptr.lock();
		Text* tx1 = new Text(*ptr, "assets/font/Call me maybe.ttf", 50, Text::BLENDED, "Press ESC To Exit", {255, 50, 50, 255});
		ptr->box.Centered({512, 400});
		ptr->AddComponent(tx1);
	} else {
		// End img.
		GameObject *go = new GameObject();
		weak_ptr = AddObject(go);
		ptr = weak_ptr.lock();
		ptr->box.x = 0;
		ptr->box.y = 0;
		Sprite *sp = new Sprite(*ptr, "assets/img/lose.jpg");
		CameraFollower *cmfl = new CameraFollower(*ptr);
		ptr->AddComponent(sp);
		ptr->AddComponent(cmfl);

		// BGM
		bgMusic.Open("assets/audio/endStateLose.ogg");
	
		// End txt.
		GameObject *tgo = new GameObject();
		weak_ptr = AddObject(tgo);
		ptr = weak_ptr.lock();
		Text* tx = new Text(*ptr, "assets/font/Call me maybe.ttf", 50, Text::BLENDED, "Press Space To Play Again", {50, 50, 255, 255});
		ptr->box.Centered({512, 100});
		ptr->AddComponent(tx);

		GameObject *tgo1 = new GameObject();
		weak_ptr = AddObject(tgo1);
		ptr = weak_ptr.lock();
		Text* tx1 = new Text(*ptr, "assets/font/Call me maybe.ttf", 50, Text::BLENDED, "Press ESC To Exit", {255, 50, 50, 255});
		ptr->box.Centered({512, 175});
		ptr->AddComponent(tx1);
	}

}

EndState::~EndState() {
	objectArray.clear();
}

void EndState::Update(float dt) {
	InputManager& input = InputManager::GetInstance();

	// Fecha o jogo.
	if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
		quitRequested = true;
	}

	// Inicia o jogo.
	if (input.KeyPress(SPACE_KEY)) { 
		popRequested = true;
	}

	UpdateArray(dt);
}

void EndState::Render() {
	RenderArray();
}

void EndState::LoadAssets() {

}

void EndState::Start(){
	LoadAssets();
	
	StartArray();

	bgMusic.Play(-1);

	started = true;
}

void EndState::Pause() {

}

void EndState::Resume() {
	Camera::pos.x = 0;
	Camera::pos.y = 0;
}