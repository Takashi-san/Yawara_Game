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

#define TITLE_STT_BG		"assets/img/background/main_menu.png"
#define TITLE_STT_BGCTRLS	"assets/img/background/menu_controles.png"
#define TITLE_STT_PLAY		"assets/img/text/jogar.png"
#define TITLE_STT_QUIT		"assets/img/text/sair.png"
#define TITLE_STT_CTRLS		"assets/img/text/controles.png"
#define TITLE_STT_PLAY_GLW	"assets/img/text/jogar_brilho.png"
#define TITLE_STT_QUIT_GLW	"assets/img/text/sair_brilho.png"
#define TITLE_STT_CTRLS_GLW	"assets/img/text/controles_brilho.png"
#define TITLE_STT_CURSOR	"assets/img/cursor/cursor.png"

#define YWR_SLEEPING		"assets/img/yawara/yawara_sleeping.png"
#define YWR_SLEEPING_FRAMES	4
#define YWR_SLEEP_FRAMETIME	0.35

#define TITLE_STT_BGM			"assets/audio/musica/main_menu.ogg"
#define TITLE_STT_SLCT_SOUND	"assets/audio/sons/menu-selecionar_opcao.ogg"
#define TITLE_STT_CHNG_SLCT		"assets/audio/sons/menu-mover.ogg"

#define TITLE_STT_BLACK 			"assets/img/background/tela_preta.png"
#define TITLE_STT_WHITE 			"assets/img/background/tela_branca.png"
#define TITLE_STT_FADE 				3

TitleState::TitleState() {
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;

	float offset = 80;

	// Title img.
	GameObject *go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	ptr->box.x = 0;
	ptr->box.y = 0;
	Sprite *sp = new Sprite(*ptr, TITLE_STT_BG);
	sp->SetFullscreen();
	CameraFollower *cmfl = new CameraFollower(*ptr);
	ptr->AddComponent(sp);
	ptr->AddComponent(cmfl);

	Vec2 scale = Vec2(0.5, 0.5);
	Vec2 ywrpos(500*sp->GetScale().x, 430*sp->GetScale().y);

	// yawara
	GameObject *ywrgo = new GameObject();
	weak_ptr = AddObject(ywrgo);
	ptr = weak_ptr.lock();
	Sprite *ywrsleep = new Sprite(*ptr, YWR_SLEEPING, YWR_SLEEPING_FRAMES, YWR_SLEEP_FRAMETIME);
	ptr->box.Centered(ywrpos);
	ptr->AddComponent(ywrsleep);

	// Title txt.
	GameObject *t1go = new GameObject();
	weak_ptr = AddObject(t1go);
	ptr = weak_ptr.lock();
	Sprite* tx1 = new Sprite(*ptr, TITLE_STT_PLAY);
	Sprite* glw1 = new Sprite(*ptr, TITLE_STT_PLAY_GLW);
	tx1->SetScale(scale);
	glw1->SetScale(scale);
	Vec2 pos1 = {((float) tx1->GetWidth())/2 + offset, Game::GetInstance().GetWindowSize().y - tx1->GetHeight()};
	ptr->box.Centered(pos1);
	ptr->AddComponent(glw1);
	ptr->AddComponent(tx1);

	GameObject *t2go = new GameObject();
	weak_ptr = AddObject(t2go);
	ptr = weak_ptr.lock();
	Sprite* tx2 = new Sprite(*ptr, TITLE_STT_CTRLS);
	Sprite* glw2 = new Sprite(*ptr, TITLE_STT_CTRLS_GLW);
	tx2->SetScale(scale);
	glw2->SetScale(scale);
	Vec2 pos2 = {(Game::GetInstance().GetWindowSize().x/2), Game::GetInstance().GetWindowSize().y - tx1->GetHeight()};
	ptr->box.Centered(pos2);
	ptr->AddComponent(glw2);
	ptr->AddComponent(tx2);

	GameObject *t3go = new GameObject();
	weak_ptr = AddObject(t3go);
	ptr = weak_ptr.lock();
	Sprite* tx3 = new Sprite(*ptr, TITLE_STT_QUIT);
	Sprite* glw3 = new Sprite(*ptr, TITLE_STT_QUIT_GLW);
	tx3->SetScale(scale);
	glw3->SetScale(scale);
	Vec2 pos3 = {Game::GetInstance().GetWindowSize().x - ((float)tx3->GetWidth())/2 - offset, Game::GetInstance().GetWindowSize().y - tx1->GetHeight()};
	ptr->box.Centered(pos3);
	ptr->AddComponent(glw3);
	ptr->AddComponent(tx3);

	// Selection.
	GameObject *sgo = new GameObject();
	weak_ptr = AddObject(sgo);
	ptr = weak_ptr.lock();
	selection = sgo;
	Sprite* txs = new Sprite(*ptr, TITLE_STT_CURSOR);
	txs->SetScale(1.5, 1.5);
	xpos[0] = pos1.x - tx1->GetWidth()/2 - txs->GetWidth()/2;
	xpos[1] = pos2.x - tx2->GetWidth()/2 - txs->GetWidth()/2;
	xpos[2] = pos3.x - tx3->GetWidth()/2 - txs->GetWidth()/2;
	ypos = pos1.y;
	ptr->box.Centered(xpos[0], ypos);
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

	//Controls screen
	GameObject* controls_go = new GameObject();
	weak_ptr = AddObject(controls_go);
	controlsgo = weak_ptr.lock();

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
	static CameraFollower* cmfll = nullptr;
	static Sprite* controlScreen = nullptr;

	static bool flag = false;
	static Timer fadein;

	if (black && flag) {
		fadein.Update(dt);
		
		black->SetAlphaMod(255 * QuadraticEaseIn(fadein.Get() / TITLE_STT_FADE));

		if (fadein.Get() > TITLE_STT_FADE) {
			fadein.Restart();
			flag = false;
			StageState *stage = nullptr;
			stage = new StageState();
			Game::GetInstance().Push(stage);
			if(play)
				play->Play(1);
		}
	}

	// Fecha o jogo.
	if (input.QuitRequested()) {
		quitRequested = true;
	}

	if(!controlScreen) {
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
		if(input.KeyPress(ESCAPE_KEY))
			quitRequested = true;
	} else {
		if(input.KeyPress(ESCAPE_KEY)){
			controlsgo->RemoveComponent(controlScreen);
			controlsgo->RemoveComponent(cmfll);
			cmfll = nullptr;
			controlScreen = nullptr;
			if(select)
				select->Play(1);
		}
	}

	switch (opt)
	{
		case PLAY:
			selection->box.Centered(xpos[0], ypos);
		break;

		case OPTIONS:
			selection->box.Centered(xpos[1], ypos);
		break;

		case QUIT:
			selection->box.Centered(xpos[2], ypos);
		break;
		
		default:
		break;
	}

	if (input.KeyPress(ENTER_KEY) || input.KeyPress(ENTER_KEY2)) {
		StageState *stage = nullptr;
		switch (opt) {
			case PLAY:
				flag = true;
				if(selectionSprite)
					selectionSprite->SetAlphaMod(255);
			break;

			case QUIT:
				quitRequested = true;
				if(select)
					select->Play(1);
			break;

			case OPTIONS:
				if(!controlScreen){
					controlScreen = new Sprite(*controlsgo, TITLE_STT_BGCTRLS);
					controlScreen->SetFullscreen();
					cmfll = new CameraFollower(*controlsgo);
					controlsgo->AddComponent(cmfll);
					controlsgo->AddComponent(controlScreen);
					if(select)
						select->Play(1);
				}
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

	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;
	
	// Black screen;
	GameObject* go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	black = new Sprite(*ptr, TITLE_STT_BLACK);
	CameraFollower* cmfr = new CameraFollower(*ptr);
	ptr->AddComponent(black);
	ptr->AddComponent(cmfr);
	black->SetAlphaMod(0);

	// White screen;
	go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	white = new Sprite(*ptr, TITLE_STT_WHITE);
	cmfr = new CameraFollower(*ptr);
	ptr->AddComponent(white);
	ptr->AddComponent(cmfr);
	white->SetAlphaMod(0);

	bgMusic.Play();

	started = true;
}

void TitleState::Pause() {

}

void TitleState::Resume() {
	Camera::pos.x = 0;
	Camera::pos.y = 0;

	if (black) {
		black->SetAlphaMod(0);
		white->SetAlphaMod(0);
	}

	bgMusic.Play();
}