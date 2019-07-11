#include "PauseState.h"
#include "StageState.h"
#include "Game.h"
#include "CameraFollower.h"
#include "InputManager.h"
#include "Camera.h"
#include "Text.h"
#include "Timer.h"

#include "Fonts.h"

#define PAUSE_STT_BG	"assets/img/background/main_menu.png"
#define PAUSE_STT_CONT	"assets/img/text/continuar.png"
#define PAUSE_STT_QUIT	"assets/img/text/main_menu_sair.png"
#define CURSOR_PATH		"assets/img/cursor/cursor.png"

#define PAUSE_STT_SLCT_SOUND	"assets/audio/sons/menu-selecionar_opcao.ogg"
#define PAUSE_STT_CHNG_SLCT		"assets/audio/sons/menu-mover.ogg"
#define PAUSE_STT_PLAY_SOUND	"assets/audio/sons/menu-jogar.ogg"

PauseState::PauseState()
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
	Sprite *sp = new Sprite(*ptr, PAUSE_STT_BG);
	sp->SetFullscreen();
	ptr->AddComponent(sp);

	// Pause txt.
	GameObject *t1go = new GameObject();
	weak_ptr = AddObject(t1go);
	ptr = weak_ptr.lock();
	Sprite *tx1 = new Sprite(*ptr, PAUSE_STT_CONT);
	tx1->SetScale(0.4, 0.4);
	Vec2 pos1 = {((float) tx1->GetWidth())/2 + offset + Camera::pos.x, Game::GetInstance().GetWindowSize().y - tx1->GetHeight() + Camera::pos.y};
	ptr->box.Centered(pos1);
	ptr->AddComponent(tx1);

	GameObject *t2go = new GameObject();
	weak_ptr = AddObject(t2go);
	ptr = weak_ptr.lock();
	Sprite *tx2 = new Sprite(*ptr, PAUSE_STT_QUIT);
	tx2->SetScale(0.3, 0.3);
	Vec2 pos2 = {Game::GetInstance().GetWindowSize().x - ((float)tx2->GetWidth())/2 - offset + Camera::pos.x, Game::GetInstance().GetWindowSize().y - tx1->GetHeight() + Camera::pos.y};
	ptr->box.Centered(pos2);
	ptr->AddComponent(tx2);

	// Selection.
	GameObject *sgo = new GameObject();
	weak_ptr = AddObject(sgo);
	ptr = weak_ptr.lock();
	selection = sgo;
	Sprite *sspr = new Sprite(*ptr, CURSOR_PATH);
	xpos[0] = pos1.x - tx1->GetWidth()/2 - sspr->GetWidth()/2;
	xpos[1] = pos2.x - tx2->GetWidth()/2 - sspr->GetWidth()/2;
	ypos = pos1.y;
	ptr->box.Centered({xpos[0], ypos});
	ptr->AddComponent(sspr);

	// sounds
	GameObject *chngslctgo = new GameObject();
	weak_ptr = AddObject(chngslctgo);
	ptr = weak_ptr.lock();
	changeSelection = new Sound(*ptr, PAUSE_STT_CHNG_SLCT);

	GameObject *slctgo = new GameObject();
	weak_ptr = AddObject(slctgo);
	ptr = weak_ptr.lock();
	select = new Sound(*ptr, PAUSE_STT_SLCT_SOUND);

	GameObject *playgo = new GameObject();
	weak_ptr = AddObject(playgo);
	ptr = weak_ptr.lock();
	play = new Sound(*ptr, PAUSE_STT_PLAY_SOUND);

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