#include "EndState.h"
#include "Data.h"
#include "Sprite.h"
#include "CameraFollower.h"
#include "Text.h"
#include "Camera.h"
#include "InputManager.h"

#define LOSE_END "assets/img/background/game_over.png"
#define CONT_TXT "assets/img/text/continuar.png"
#define NEWG_TXT "assets/img/text/novo_jogo.png"
#define WIN_MUSIC ""
#define LOSE_MUSIC "assets/audio/musica/tema_triste.ogg"

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
		bgMusic.Open(WIN_MUSIC);
	
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
		Sprite *sp = new Sprite(*ptr, LOSE_END);
		CameraFollower *cmfl = new CameraFollower(*ptr);
		ptr->AddComponent(sp);
		ptr->AddComponent(cmfl);

		// BGM
		bgMusic.Open(LOSE_MUSIC);
	
		// End txt.
		GameObject *tgo = new GameObject();
		weak_ptr = AddObject(tgo);
		ptr = weak_ptr.lock();
		Sprite* tx = new Sprite(*ptr, CONT_TXT);
		tx->SetScale({0.3,0.3});
		ptr->box.Centered({280, 500});
		ptr->AddComponent(tx);

		GameObject *tgo1 = new GameObject();
		weak_ptr = AddObject(tgo1);
		ptr = weak_ptr.lock();
		Sprite* tx1 = new Sprite(*ptr, NEWG_TXT);
		tx1->SetScale({0.3,0.3});
		ptr->box.Centered({683, 500});
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

	bgMusic.Play();

	started = true;
}

void EndState::Pause() {

}

void EndState::Resume() {
	Camera::pos.x = 0;
	Camera::pos.y = 0;
}