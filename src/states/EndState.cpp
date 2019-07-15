#include "EndState.h"
#include "Data.h"
#include "Sprite.h"
#include "CameraFollower.h"
#include "Text.h"
#include "Camera.h"
#include "InputManager.h"
#include "Game.h"
#include "Easing.h"

#define END_STT_BG			"assets/img/background/game_over.png"
#define END_STT_QUIT		"assets/img/text/sair.png"
#define END_STT_NEWG		"assets/img/text/novo_jogo.png"
#define END_STT_QUIT_GLW	"assets/img/text/sair_brilho.png"
#define END_STT_NEWG_GLW	"assets/img/text/novo_jogo_brilho.png"
#define WIN_MUSIC			""
#define LOSE_MUSIC			"assets/audio/musica/tema_triste.ogg"

#define END_STT_SLCT_SOUND	"assets/audio/sons/menu-selecionar_opcao.ogg"
#define END_STT_CHNG_SLCT	"assets/audio/sons/menu-mover.ogg"

#define CURSOR_PATH "assets/img/cursor/cursor.png"

EndState::EndState() {
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;

	float offset = 80;

	Camera::pos = {0,  0};

	Opt = NEWGAME;

	// sounds
	GameObject *chngslctgo = new GameObject();
	weak_ptr = AddObject(chngslctgo);
	ptr = weak_ptr.lock();
	changeSelection = new Sound(*ptr, END_STT_CHNG_SLCT);

	GameObject *slctgo = new GameObject();
	weak_ptr = AddObject(slctgo);
	ptr = weak_ptr.lock();
	select = new Sound(*ptr, END_STT_SLCT_SOUND);

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

		Vec2 scale(0.5, 0.5);

		// BGM
		bgMusic.Open(LOSE_MUSIC);
	
		// End txt.
		GameObject *t1go = new GameObject();
		weak_ptr = AddObject(t1go);
		ptr = weak_ptr.lock();
		Sprite *tx1 = new Sprite(*ptr, END_STT_NEWG);
		Sprite *glw1 = new Sprite(*ptr, END_STT_NEWG_GLW);
		tx1->SetScale(scale);
		glw1->SetScale(scale);
		Vec2 pos1 = {((float) tx1->GetWidth())/2 + offset, Game::GetInstance().GetWindowSize().y - tx1->GetHeight()};
		ptr->box.Centered(pos1);
		ptr->AddComponent(tx1);

		GameObject *t2go = new GameObject();
		weak_ptr = AddObject(t2go);
		ptr = weak_ptr.lock();
		Sprite *tx2 = new Sprite(*ptr, END_STT_QUIT);
		Sprite *glw2 = new Sprite(*ptr, END_STT_QUIT_GLW);
		tx2->SetScale(scale);
		glw2->SetScale(scale);
		Vec2 pos2 = {Game::GetInstance().GetWindowSize().x - ((float)tx2->GetWidth())/2 - offset, Game::GetInstance().GetWindowSize().y - tx1->GetHeight()};
		ptr->box.Centered(pos2);
		ptr->AddComponent(tx2);

		//Selection
		GameObject *sgo = new GameObject();
		weak_ptr = AddObject(sgo);
		ptr = weak_ptr.lock();
		selection = sgo;
		selectionSprite = new Sprite(*ptr, CURSOR_PATH);
		selectionSprite->SetScale(1.5, 1.5);
		xpos[0] = pos1.x - tx1->GetWidth()/2 - selectionSprite->GetWidth()/2;
		xpos[1] = pos2.x - tx2->GetWidth()/2 - selectionSprite->GetWidth()/2;
		ypos = pos1.y;
		ptr->box.Centered({xpos[0], ypos});
		ptr->AddComponent(selectionSprite);
	}

}

EndState::~EndState() {
	objectArray.clear();
}

void EndState::Update(float dt) {
	InputManager& input = InputManager::GetInstance();
	static float counter = 0;
	static bool fadingIn = true;
	static float ease = 0.3;

	// Fecha o jogo.
	if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
		quitRequested = true;
	}

	if(input.KeyPress(LEFT_ARROW_KEY) || input.KeyPress(A_KEY)) {
		
		if(Opt != NEWGAME) {
			Opt = NEWGAME;
			if(changeSelection)
				changeSelection->Play();
		}
	}

	if(input.KeyPress(RIGHT_ARROW_KEY) || input.KeyPress(D_KEY)) {
		if(Opt != QUIT) {
			Opt = QUIT;
			if(changeSelection)
				changeSelection->Play();
		}
	}

	switch (Opt)
	{
	case NEWGAME:
		selection->box.Centered({xpos[0], ypos});
		break;
	
	case QUIT:
		selection->box.Centered({xpos[1], ypos});
		break;
	}

	if(input.KeyPress(ENTER_KEY) || input.KeyPress(ENTER_KEY2)){

		if(select)
				select->Play();

		switch (Opt)
		{
		case NEWGAME:
			popRequested = true;

			if(selectionSprite)
				selectionSprite->SetAlphaMod(255);

			break;

		case QUIT:
			quitRequested = true;
			break;
		
		default:
			break;
		}
	}

	if(selectionSprite){
		if(fadingIn){
			counter += dt;
			if(counter >= 1){
				counter = 1;
				fadingIn = false;
			}
		} else{
			counter -= dt;
			if(counter <= 0.3){
				counter = 0.3;
				fadingIn = true;
			}
		}

		ease = SineEaseInOut(counter);

		selectionSprite->SetAlphaMod(int (ease * 255));
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