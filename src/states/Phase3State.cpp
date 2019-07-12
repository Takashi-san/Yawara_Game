#include "Phase3State.h"

#include "PauseState.h"
#include "Sprite.h"
#include "Sound.h"
#include "TileMap.h"
#include "Vec2.h"
#include "InputManager.h"
#include "Camera.h"
#include "CameraFollower.h"
#include "Capelobo.h"
#include "Collision.h"
#include "Collider.h"
#include "Data.h"
#include "EndState.h"
#include "Game.h"
#include "Cursor.h"
#include "Yawara.h"
#include "Floor.h"
#include "HealthRune.h"
#include "AttackRune.h"
#include "DefenseRune.h"
#include "HealthFeedback.h"
#include "YawaraShadow.h"
#include "Timer.h"
#include "Easing.h"
#include "MapColision.h"
#include "CreditosState.h"

#include "Tilesets.h"
#include "Tilemaps.h"

#define PHASE2_STT_BG "assets/img/background/nuvem/back.png"
#define PHASE2_STT_BGM "assets/audio/musica/musica_baiao.ogg"
#define PHASE2_STT_CURSOR_SPRITE "assets/img/cursor/cursor.png"
#define PHASE2_STT_CAMERA_RATIO 0.37

#define PHASE2_STT_BLACK 	"assets/img/background/tela_preta.png"
#define PHASE2_STT_WHITE 	"assets/img/background/tela_branca.png"
#define PHASE2_STT_FADE 	6

Phase3State::Phase3State()
{
	std::cout << "BEM VINDO A FASE 3\n";
	Data::lose = false;
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;

	// Background
	GameObject *gobg = new GameObject();
	weak_ptr = AddObject(gobg);
	ptr = weak_ptr.lock();
	ptr->box.x = 0;
	ptr->box.y = 0;
	Sprite *sp = new Sprite(*ptr, PHASE2_STT_BG);
	sp->SetFullscreen();
	CameraFollower *cmfl = new CameraFollower(*ptr);
	ptr->AddComponent(sp);
	ptr->AddComponent(cmfl);

	// TileMap
	GameObject* tmpgo = new GameObject();
	TileMap* tmpmap = new TileMap(*tmpgo);

	// Fase3
	TileMap::SetMapLayer(*this, "assets/tilemap/fase_3/camada_1_solo", 4, 9, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/fase_3/camada_2_solo", 4, 9, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/fase_3/camada_3_solo", 4, 9, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/fase_3/camada_4_solo", 4, 9, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/fase_3/camada_5_solo", 4, 9, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);

	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_6_arvorevoid3", 4, 9, 80, 80, TS_ARVORE3V, TS_ARVORE3V_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_7_arvorevoid2", 4, 9, 80, 80, TS_ARVORE2V, TS_ARVORE2V_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_8_arvorevoid1", 4, 9, 80, 80, TS_ARVORE1V, TS_ARVORE1V_F, TS_FRAME_TIME);

	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_9_arvorecerrado2", 4, 9, 80, 80, TS_ARVORE2C, TS_ARVORE2C_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_10_arvorecerrado3", 4, 9, 80, 80, TS_ARVORE3C, TS_ARVORE3C_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_11_arvorecerrado1", 4, 9, 80, 80, TS_ARVORE1C, TS_ARVORE1C_F, TS_FRAME_TIME);

	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_12_arvoremata3", 4, 9, 80, 80, TS_ARVORE3M, TS_ARVORE3M_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_13_arvoremata1", 4, 9, 80, 80, TS_ARVORE1M, TS_ARVORE1M_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_14_arvoremata2", 4, 9, 80, 80, TS_ARVORE2M, TS_ARVORE2M_F, TS_FRAME_TIME);

	TileMap::SetMapLayer(*this, "assets/tilemap/fase_3/camada_15_pedra", 4, 9, 80, 80, TS_PEDRA, TS_PEDRA_W, TS_PEDRA_H);

	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_16_planta1", 4, 9, 80, 80, TS_PLANTA1V, TS_PLANTA1V_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_17_planta2", 4, 9, 80, 80, TS_PLANTA2V, TS_PLANTA2V_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_18_planta3", 4, 9, 80, 80, TS_PLANTA3V, TS_PLANTA3V_F, TS_FRAME_TIME);

	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_19_planta4", 4, 9, 80, 80, TS_PLANTA1C, TS_PLANTA1C_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_20_planta5", 4, 9, 80, 80, TS_PLANTA2C, TS_PLANTA2C_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_21_planta6", 4, 9, 80, 80, TS_PLANTA3C, TS_PLANTA3C_F, TS_FRAME_TIME);

	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_22_planta7", 4, 9, 80, 80, TS_PLANTA1M, TS_PLANTA1M_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_23_planta8", 4, 9, 80, 80, TS_PLANTA2M, TS_PLANTA2M_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_3/camada_24_planta9", 4, 9, 80, 80, TS_PLANTA3M, TS_PLANTA3M_F, TS_FRAME_TIME);

	TileMap::SetMapLayer(*this, "assets/tilemap/fase_3/camada_25_runa", 4, 9, 80, 80, TS_RUNA, TS_RUNA_W, TS_RUNA_H);

	TileMap::SetMapLayer(*this, "assets/tilemap/fase_3/camada_28_luz", 4, 9, 80, 80, TS_LUZ, TS_LUZ_W, TS_LUZ_H);

	MapColision::GetInstance().SetMapColision("assets/tilemap/fase_3/camada_29_colisao", 4, 9, 80, 80);
	//TileMap::SetMapLayer(*this, "assets/tilemap/fase_3/camada_29_colisao", 4, 9, 80, 80, TS_DEBUG, TS_DEBUG_W, TS_DEBUG_H);

	// HealthFeedback.
	GameObject* go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	HealthFeedback* hfb = new HealthFeedback(*ptr);
	ptr->AddComponent(hfb);

	// YawaraShadow.
	go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	YawaraShadow* ysh = new YawaraShadow(*ptr);
	ptr->AddComponent(ysh);

	// Yawara
	GameObject *goya = new GameObject();
	weak_ptr = AddObject(goya);
	ptr = weak_ptr.lock();
	Yawara *yawara = new Yawara(*ptr);
	ptr->box.Centered({Data::pos.x, Data::pos.y});
	ptr->AddComponent(yawara);

	// Cursor
	GameObject *gocur = new GameObject();
	weak_ptr = AddObject(gocur);
	ptr = weak_ptr.lock();
	Cursor *cur = new Cursor(*ptr);
	Sprite *spcur = new Sprite(*ptr, PHASE2_STT_CURSOR_SPRITE);
	spcur->SetScale(1, 1);
	ptr->AddComponent(cur);
	ptr->AddComponent(spcur);

	Camera::Follow(GetObjectPtr(goya), weak_ptr);
	Camera::ratio = PHASE2_STT_CAMERA_RATIO;

	// BGM
	bgMusic.Open(PHASE2_STT_BGM);
	bgMusic.Play(-1, 30);

	flag = true;
	ignore = false;
}

Phase3State::~Phase3State()
{
	objectArray.clear();
}

void Phase3State::Update(float dt)
{
	static Timer fadein;
	static bool flag = true;

	if (white && flag) {
		fadein.Update(dt);
		
		white->SetAlphaMod(255 * (1 - QuadraticEaseIn(fadein.Get() / PHASE2_STT_FADE)));

		if (fadein.Get() > PHASE2_STT_FADE) {
			white->SetAlphaMod(0);
			fadein.Restart();
			flag = false;
		}
	}

	InputManager &input = InputManager::GetInstance();

	// verifica fechamento do jogo.
	if (input.KeyPress(ESCAPE_KEY))
	{
		CreditosState *stage = new CreditosState();
		Game::GetInstance().Push(stage);
	}

	if(input.QuitRequested()){
		quitRequested = true;
	}

	/*
	// verifica condições de vitoria.
	if ((Yawara::player == nullptr) && !ignore)
	{
		popRequested = true;
		Data::playerVictory = false;
		EndState *stage = new EndState();
		Game::GetInstance().Push(stage);
	}
	else if (Capelobo::boss == nullptr)
	{
		if (white) {
			fadein.Update(dt);
			
			white->SetAlphaMod(255 * QuadraticEaseIn(fadein.Get() / PHASE2_STT_FADE));

			if (fadein.Get() > PHASE2_STT_FADE) {
				popRequested = true;
				ignore = true;
				//Data::playerVictory = true;
				//EndState *stage = new EndState();
				//Game::GetInstance().Push(stage);
			}
		}

	}
	*/

	UpdateArray(dt);

	// update dos colliders.
	for (unsigned int i = 0; i < objectArray.size(); i++)
	{
		Collider *coli = static_cast<Collider *>(objectArray[i]->GetComponent("Collider"));
		if (coli != nullptr)
		{
			coli->Update(dt);
		}
	}

	// verifica colisão dos objetos.
	for (unsigned int i = 0; i < objectArray.size(); i++) {
		Collider *coli = static_cast<Collider*>(objectArray[i]->GetComponent("Collider"));
		if (coli != nullptr) {
			for (unsigned int j = i+1; j < objectArray.size(); j++) {
				Collider *colj = static_cast<Collider*>(objectArray[j]->GetComponent("Collider"));
				if (colj != nullptr) {
					if (Collision::IsColliding(coli->box, colj->box, objectArray[i]->angleDeg/0.0174533, objectArray[j]->angleDeg/0.0174533)) {
						objectArray[i]->NotifyCollision(*(objectArray[j]));
						objectArray[j]->NotifyCollision(*(objectArray[i]));
					}
				}
			}
		}
	}

	Camera::Update(dt);

	// Componentes dependentes de Camera.
	// update dos CameraFollower.
	for (unsigned int i = 0; i < objectArray.size(); i++)
	{
		Collider *coli = static_cast<Collider *>(objectArray[i]->GetComponent("CameraFollower"));
		if (coli != nullptr)
		{
			coli->Update(dt);
			break;
		}
	}

	// update dos Cursor.
	for (unsigned int i = 0; i < objectArray.size(); i++)
	{
		Collider *coli = static_cast<Collider *>(objectArray[i]->GetComponent("Cursor"));
		if (coli != nullptr)
		{
			coli->Update(dt);
			break;
		}
	}
}

void Phase3State::Render()
{
	RenderArray();
}

void Phase3State::LoadAssets()
{
}

void Phase3State::Start()
{
	LoadAssets();

	StartArray();

	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;
	
	// Black screen;
	GameObject* go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	black = new Sprite(*ptr, PHASE2_STT_BLACK);
	CameraFollower* cmfr = new CameraFollower(*ptr);
	ptr->AddComponent(black);
	ptr->AddComponent(cmfr);
	black->SetAlphaMod(0);

	// White screen;
	go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	white = new Sprite(*ptr, PHASE2_STT_WHITE);
	cmfr = new CameraFollower(*ptr);
	ptr->AddComponent(white);
	ptr->AddComponent(cmfr);

	started = true;

	// nuvens.
	go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	Sprite* nuvem = new Sprite(*ptr, "assets/img/background/nuvem/1.png");
	ptr->AddComponent(nuvem);
	ptr->box.y = 1280*0;

	go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	nuvem = new Sprite(*ptr, "assets/img/background/nuvem/2.png");
	ptr->AddComponent(nuvem);
	ptr->box.y = 1280*1;

	go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	nuvem = new Sprite(*ptr, "assets/img/background/nuvem/3.png");
	ptr->AddComponent(nuvem);
	ptr->box.y = 1280*2;

	go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	nuvem = new Sprite(*ptr, "assets/img/background/nuvem/4.png");
	ptr->AddComponent(nuvem);
	ptr->box.y = 1280*3;

	go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	nuvem = new Sprite(*ptr, "assets/img/background/nuvem/5.png");
	ptr->AddComponent(nuvem);
	ptr->box.y = 1280*4;

	go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	nuvem = new Sprite(*ptr, "assets/img/background/nuvem/6.png");
	ptr->AddComponent(nuvem);
	ptr->box.y = 1280*5;

	go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	nuvem = new Sprite(*ptr, "assets/img/background/nuvem/7.png");
	ptr->AddComponent(nuvem);
	ptr->box.y = 1280*6;

	go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	nuvem = new Sprite(*ptr, "assets/img/background/nuvem/8.png");
	ptr->AddComponent(nuvem);
	ptr->box.y = 1280*7;

	go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	nuvem = new Sprite(*ptr, "assets/img/background/nuvem/9.png");
	ptr->AddComponent(nuvem);
	ptr->box.y = 1280*8;

	//Runas
	GameObject *gorune = new GameObject();
	weak_ptr = AddObject(gorune);
	ptr = weak_ptr.lock();
	HealthRune *rune1 = new HealthRune(*ptr, 1.5);
	ptr->box.Centered(3090,1780);
	ptr->AddComponent(rune1);

	gorune = new GameObject();
	weak_ptr = AddObject(gorune);
	ptr = weak_ptr.lock();
	rune1 = new HealthRune(*ptr, 1.5);
	ptr->box.Centered(1850,890);
	ptr->AddComponent(rune1);
	
	gorune = new GameObject();
	weak_ptr = AddObject(gorune);
	ptr = weak_ptr.lock();
	AttackRune* attrune = new AttackRune(*ptr, 1.2, AttackRune::INIMIGO);
	ptr->box.x = 3312;
	ptr->box.y = 117;
	ptr->AddComponent(attrune);

	gorune = new GameObject();
	weak_ptr = AddObject(gorune);
	ptr = weak_ptr.lock();
	attrune = new AttackRune(*ptr, 1.3, AttackRune::YAWARA);
	ptr->box.x = 4544;
	ptr->box.y = 1701;
	ptr->AddComponent(attrune);

	gorune = new GameObject();
	weak_ptr = AddObject(gorune);
	ptr = weak_ptr.lock();
	attrune = new AttackRune(*ptr, 1.4, AttackRune::ENCONTRO);
	ptr->box.x = 160;
	ptr->box.y = 2901;
	ptr->AddComponent(attrune);

	gorune = new GameObject();
	weak_ptr = AddObject(gorune);
	ptr = weak_ptr.lock();
	attrune = new AttackRune(*ptr, 1.5, AttackRune::AJUDA);
	ptr->box.x = 288;
	ptr->box.y = 3932;
	ptr->AddComponent(attrune);

	gorune = new GameObject();
	weak_ptr = AddObject(gorune);
	ptr = weak_ptr.lock();
	attrune = new AttackRune(*ptr, 1.2, AttackRune::INIMIGO);
	ptr->box.x = 1312;
	ptr->box.y = 5173;
	ptr->AddComponent(attrune);

	gorune = new GameObject();
	weak_ptr = AddObject(gorune);
	ptr = weak_ptr.lock();
	attrune = new AttackRune(*ptr, 1.4, AttackRune::ENCONTRO);
	ptr->box.x = 2000;
	ptr->box.y = 9797;
	ptr->AddComponent(attrune);

	bgMusic.Play(-1, 30);
}

void Phase3State::Pause()
{
}

void Phase3State::Resume()
{
}