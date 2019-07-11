#include "EndState.h"
#include "Data.h"
#include "Sprite.h"
#include "CameraFollower.h"
#include "Text.h"
#include "Camera.h"
#include "InputManager.h"

#define END_STT_BG		"assets/img/background/game_over.png"
#define END_STT_QUIT	"assets/img/text/main_menu_sair.png"
#define END_STT_NEWG	"assets/img/text/novo_jogo.png"
#define WIN_MUSIC		""
#define LOSE_MUSIC		"assets/audio/musica/tema_triste.ogg"

#define CURSOR_PATH "assets/img/cursor/cursor.png"

EndState::EndState() {
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;

	Camera::pos = {0,  0};

	Opt = NEWGAME;

	if (Data::playerVictory) {
		// End img.
	} else {
		// End img.
		GameObject *go = new GameObject();
		weak_ptr = AddObject(go);
		ptr = weak_ptr.lock();
		ptr->box.x = 0;
		ptr->box.y = 0;
		Sprite *bg = new Sprite(*ptr, END_STT_BG);
		bg->SetFullscreen();
		CameraFollower *cmfl = new CameraFollower(*ptr);
		ptr->AddComponent(bg);
		ptr->AddComponent(cmfl);

		// BGM
		bgMusic.Open(LOSE_MUSIC);
	
		// End txt.
		GameObject *t1go = new GameObject();
		weak_ptr = AddObject(t1go);
		ptr = weak_ptr.lock();
		Sprite *tx1 = new Sprite(*ptr, END_STT_QUIT);
		tx1->SetScale(0.3, 0.3);
		ptr->box.Centered(341, 500);
		ptr->AddComponent(tx1);

		GameObject *t2go = new GameObject();
		weak_ptr = AddObject(t2go);
		ptr = weak_ptr.lock();
		Sprite *tx2 = new Sprite(*ptr, END_STT_NEWG);
		tx2->SetScale(0.4, 0.4);
		ptr->box.Centered(720, 500);
		ptr->AddComponent(tx2);

		//Selection
		GameObject *sgo = new GameObject();
		weak_ptr = AddObject(sgo);
		ptr = weak_ptr.lock();
		selection = sgo;
		Sprite *sspr = new Sprite(*ptr, CURSOR_PATH);
		ptr->box.Centered({580 + Camera::pos.x, 500 + Camera::pos.y});
		ptr->AddComponent(sspr);
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

	if(input.KeyPress(LEFT_ARROW_KEY) || input.KeyPress(A_KEY)){
		if(Opt == NEWGAME)
			Opt = QUIT;
	}

	if(input.KeyPress(RIGHT_ARROW_KEY) || input.KeyPress(D_KEY)){
		if(Opt == QUIT)
			Opt = NEWGAME;
	}

	switch (Opt)
	{
	case NEWGAME:
		selection->box.Centered({580 + Camera::pos.x, 500 + Camera::pos.y});
		break;
	
	case QUIT:
		selection->box.Centered({250 + Camera::pos.x, 500 + Camera::pos.y});
		break;
	}

	if(input.KeyPress(ENTER_KEY) || input.KeyPress(ENTER_KEY2)){
		switch (Opt)
		{
		case NEWGAME:
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