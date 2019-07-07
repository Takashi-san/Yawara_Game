#include "TitleState.h"
#include "StageState.h"
#include "Game.h"
#include "CameraFollower.h"
#include "InputManager.h"
#include "Camera.h"
#include "Text.h"
#include "Timer.h"
#include "Easing.h"

#include "Fonts.h"

#define TITLE_STT_BG	"assets/img/background/main_menu.png"
#define TITLE_STT_PLAY	"assets/img/background/main_menu_jogar.png"
#define TITLE_STT_QUIT	"assets/img/background/main_menu_sair.png"
#define TITLE_STT_ARROW	"assets/img/background/main_menu_selection.png"

#define TITLE_STT_BGM			"assets/audio/musica/main_menu.mp3"
#define TITLE_STT_SLCT_SOUND	"assets/audio/sons/menu-selecionar_opcao.ogg"
#define TITLE_STT_CHNG_SLCT		"assets/audio/sons/menu-mover.ogg"
#define TITLE_STT_PLAY_SOUND	"assets/audio/sons/menu-jogar.ogg"

TitleState::TitleState() {
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;

	// Title img.
	GameObject *go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	ptr->box.x = 0;
	ptr->box.y = 0;
	Sprite *sp = new Sprite(*ptr, TITLE_STT_BG);
	CameraFollower *cmfl = new CameraFollower(*ptr);
	ptr->AddComponent(sp);
	ptr->AddComponent(cmfl);

	// Title txt.
	GameObject *t1go = new GameObject();
	weak_ptr = AddObject(t1go);
	ptr = weak_ptr.lock();
	Sprite* tx1 = new Sprite(*ptr, TITLE_STT_PLAY);
	tx1->SetScale(0.5, 0.5);
	ptr->box.Centered({200, 560});
	ptr->AddComponent(tx1);

	GameObject *t2go = new GameObject();
	weak_ptr = AddObject(t2go);
	ptr = weak_ptr.lock();
	Text* tx2 = new Text(*ptr, TTF_TEMPSITC, 50, Text::BLENDED, "Controles", {0, 0, 0, 255});
	ptr->box.Centered(500, 550);
	ptr->AddComponent(tx2);

	GameObject *t3go = new GameObject();
	weak_ptr = AddObject(t3go);
	ptr = weak_ptr.lock();
	Sprite* tx3 = new Sprite(*ptr, TITLE_STT_QUIT);
	tx3->SetScale(0.3, 0.3);
	ptr->box.Centered({800, 550});
	ptr->AddComponent(tx3);

	// Selection.
	GameObject *sgo = new GameObject();
	weak_ptr = AddObject(sgo);
	ptr = weak_ptr.lock();
	selection = sgo;
	Sprite* txs = new Sprite(*ptr, TITLE_STT_ARROW);
	txs->SetScale(0.7, 0.7);
	ptr->box.Centered({90, 550});
	ptr->AddComponent(txs);

	selectionSprite = txs;

	// BGM
	bgMusic.Open(TITLE_STT_BGM);
	bgMusic.Play();

	//Sounds
	GameObject *chngslctgo = new GameObject();
	weak_ptr = AddObject(chngslctgo);
	ptr = weak_ptr.lock();
	changeSelection = new Sound(*ptr, TITLE_STT_CHNG_SLCT);

	GameObject *slctgo = new GameObject();
	weak_ptr = AddObject(slctgo);
	ptr = weak_ptr.lock();
	select = new Sound(*ptr, TITLE_STT_SLCT_SOUND);

	GameObject *playgo = new GameObject();
	weak_ptr = AddObject(playgo);
	ptr = weak_ptr.lock();
	play = new Sound(*ptr, TITLE_STT_PLAY_SOUND);

	opt = PLAY;
}

TitleState::~TitleState() {
	objectArray.clear();
}

void TitleState::Update(float dt) {
	InputManager& input = InputManager::GetInstance();
	static float counter = 0;
	static bool fadingIn = true;
	static float ease = 0.3;

	// Fecha o jogo.
	if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
		quitRequested = true;
	}

	if (input.KeyPress(A_KEY) || input.KeyPress(LEFT_ARROW_KEY)) {
		if(opt == OPTIONS)
			opt = PLAY;
		else if(opt == QUIT)
			opt = OPTIONS;

	if(changeSelection)
		changeSelection->Play(1);
		
	}
	if (input.KeyPress(D_KEY) || input.KeyPress(RIGHT_ARROW_KEY)) {
		if(opt == PLAY)
			opt = OPTIONS;
		else if(opt == OPTIONS)
			opt = QUIT;

		if(changeSelection)
			changeSelection->Play(1);
	}

	switch (opt)
	{
		case PLAY:
			selection->box.Centered({90, 550});
		break;

		case OPTIONS:
			selection->box.Centered({370, 550});
		break;

		case QUIT:
			selection->box.Centered({720, 550});
		break;
		
		default:
		break;
	}

	if (input.KeyPress(ENTER_KEY) || input.KeyPress(ENTER_KEY2)) {
		StageState *stage = new StageState();
		switch (opt) {
			case PLAY:
				Game::GetInstance().Push(stage);
				if(play)
					play->Play(1);
			break;

			case QUIT:
				quitRequested = true;
				if(select)
					select->Play(1);
			break;

			default:
				if(select)
					select->Play(1);
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

	bgMusic.Play();
}