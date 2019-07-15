#include "CreditosState.h"
#include "StageState.h"
#include "Game.h"
#include "CameraFollower.h"
#include "InputManager.h"
#include "Camera.h"
#include "Text.h"
#include "Timer.h"
#include "Easing.h"

#include "Fonts.h"

#define CREDIT_STT_BG		"assets/img/background/creditos.png"
#define CREDIT_STT_CONT		"assets/img/text/continuar.png"
#define CREDIT_STT_QUIT		"assets/img/text/sair.png"
#define CREDIT_STT_CONT_GLW	"assets/img/text/continuar_brilho.png"
#define CREDIT_STT_QUIT_GLW	"assets/img/text/sair_brilho.png"
#define CURSOR_PATH			"assets/img/cursor/cursor.png"

#define CREDIT_STT_SLCT_SOUND	"assets/audio/sons/menu-selecionar_opcao.ogg"
#define CREDIT_STT_CHNG_SLCT		"assets/audio/sons/menu-mover.ogg"
#define CREDIT_STT_PLAY_SOUND	"assets/audio/sons/menu-jogar.ogg"

CreditosState::CreditosState()
{
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;

	float offset = 80;

	// Pause img.
	GameObject *go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	ptr->box.x = Camera::pos.x;
	ptr->box.y = Camera::pos.y;
	Sprite *sp = new Sprite(*ptr, CREDIT_STT_BG);
	sp->SetFullscreen();
	ptr->AddComponent(sp);

	Vec2 scale(0.5, 0.5);

	// Pause txt.
	GameObject *t1go = new GameObject();
	weak_ptr = AddObject(t1go);
	ptr = weak_ptr.lock();
	Sprite *tx1 = new Sprite(*ptr, CREDIT_STT_CONT);
	Sprite *glw1 = new Sprite(*ptr, CREDIT_STT_CONT_GLW);
	tx1->SetScale(scale);
	glw1->SetScale(scale);
	Vec2 pos1 = {((float) tx1->GetWidth())/2 + offset + Camera::pos.x, Game::GetInstance().GetWindowSize().y - tx1->GetHeight() + Camera::pos.y};
	ptr->box.Centered(pos1);
	ptr->AddComponent(glw1);
	ptr->AddComponent(tx1);

	GameObject *t2go = new GameObject();
	weak_ptr = AddObject(t2go);
	ptr = weak_ptr.lock();
	Sprite *tx2 = new Sprite(*ptr, CREDIT_STT_QUIT);
	Sprite *glw2 = new Sprite(*ptr, CREDIT_STT_QUIT_GLW);
	tx2->SetScale(scale);
	glw2->SetScale(scale);
	Vec2 pos2 = {Game::GetInstance().GetWindowSize().x - ((float)tx2->GetWidth())/2 - offset + Camera::pos.x, Game::GetInstance().GetWindowSize().y - tx1->GetHeight() + Camera::pos.y};
	ptr->box.Centered(pos2);
	ptr->AddComponent(glw2);
	ptr->AddComponent(tx2);

	// Selection.
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

	// sounds
	GameObject *chngslctgo = new GameObject();
	weak_ptr = AddObject(chngslctgo);
	ptr = weak_ptr.lock();
	changeSelection = new Sound(*ptr, CREDIT_STT_CHNG_SLCT);

	GameObject *slctgo = new GameObject();
	weak_ptr = AddObject(slctgo);
	ptr = weak_ptr.lock();
	select = new Sound(*ptr, CREDIT_STT_SLCT_SOUND);

	GameObject *playgo = new GameObject();
	weak_ptr = AddObject(playgo);
	ptr = weak_ptr.lock();
	play = new Sound(*ptr, CREDIT_STT_PLAY_SOUND);

	// BGM

	opt = RESUME;
}

CreditosState::~CreditosState()
{
	objectArray.clear();
}

void CreditosState::Update(float dt)
{
	InputManager &input = InputManager::GetInstance();
	static float counter = 0;
	static bool fadingIn = true;
	static float ease = 0.3;

	if(input.KeyPress(ESCAPE_KEY)) {
		popRequested = true;
	}

	if(input.QuitRequested()) {
		quitRequested = true;
	}

	if (input.KeyPress(A_KEY) || input.KeyPress(LEFT_ARROW_KEY)) {
		if(opt != RESUME) {
			opt = RESUME;
			selection->box.Centered({xpos[0], ypos});
			if(changeSelection)
				changeSelection->Play();
		}
	}
	if (input.KeyPress(D_KEY) || input.KeyPress(RIGHT_ARROW_KEY)) {
		if(opt != QUIT) {
			opt = QUIT;
			selection->box.Centered({xpos[1], ypos});
			if(changeSelection)
				changeSelection->Play();
		}
	}

	if (input.KeyPress(ENTER_KEY) || input.KeyPress(ENTER_KEY2)) {
		switch (opt)
		{
		case RESUME:
			popRequested = true;
			
			if(play)
				play->Play();

			if(select)
				select->Play();
			break;

		case QUIT:
			quitRequested = true;

			if(select)
				select->Play();
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

void CreditosState::Render()
{
	RenderArray();
}

void CreditosState::LoadAssets()
{
}

void CreditosState::Start()
{
	LoadAssets();

	StartArray();

	started = true;
}

void CreditosState::Pause()
{
}

void CreditosState::Resume()
{
	Camera::pos.x = 0;
	Camera::pos.y = 0;
}