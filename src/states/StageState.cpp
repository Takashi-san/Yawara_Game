#include "StageState.h"

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
#include "HealthRune.h"
#include "AttackRune.h"
#include "DefenseRune.h"
#include "HealthFeedback.h"
#include "MapColision.h"
#include "YawaraShadow.h"
#include "Timer.h"
#include "Easing.h"
#include "Phase2State.h"
#include "Event.h"
#include "MusicEvent.h"

#include "Tilesets.h"
#include "Tilemaps.h"

#define STAGE_STT_BG "assets/img/background/nuvem/back.png"
#define STAGE_STT_BGM "assets/audio/musica/tema_triste.ogg"
#define STAGE_STT_CURSOR_SPRITE "assets/img/cursor/cursor.png"
#define STAGE_STT_CAMERA_RATIO 0.37

#define STAGE_STT_BLACK 			"assets/img/background/tela_preta.png"
#define STAGE_STT_WHITE 			"assets/img/background/tela_branca.png"
#define STAGE_STT_FADE 	6

StageState::StageState()
{
	std::cout << "BEM VINDO A FASE 1\n";
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;

	// Background
	GameObject *gobg = new GameObject();
	weak_ptr = AddObject(gobg);
	ptr = weak_ptr.lock();
	ptr->box.x = 0;
	ptr->box.y = 0;
	Sprite *sp = new Sprite(*ptr, STAGE_STT_BG);
	sp->SetFullscreen();
	CameraFollower *cmfl = new CameraFollower(*ptr);
	ptr->AddComponent(sp);
	ptr->AddComponent(cmfl);

	// TileMap
	GameObject* tmpgo = new GameObject();
	TileMap* tmpmap = new TileMap(*tmpgo);

	 // Fase1
	TileMap::SetMapLayer(*this, "assets/tilemap/fase_1/camada_1_solo", 4, 9, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/fase_1/camada_2_solo", 4, 9, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/fase_1/camada_3_solo", 4, 9, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/fase_1/camada_4_solo", 4, 9, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/fase_1/camada_5_solo", 4, 9, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);

	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_1/camada_6_arvorevoid3", 4, 9, 80, 80, TS_ARVORE3V, TS_ARVORE3V_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_1/camada_7_arvorevoid2", 4, 9, 80, 80, TS_ARVORE2V, TS_ARVORE2V_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_1/camada_8_arvorevoid1", 4, 9, 80, 80, TS_ARVORE1V, TS_ARVORE1V_F, TS_FRAME_TIME);

	TileMap::SetMapLayer(*this, "assets/tilemap/fase_1/camada_9_pedra", 4, 9, 80, 80, TS_PEDRA, TS_PEDRA_W, TS_PEDRA_H);

	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_1/camada_10_planta1", 4, 9, 80, 80, TS_PLANTA1V, TS_PLANTA1V_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_1/camada_11_planta2", 4, 9, 80, 80, TS_PLANTA2V, TS_PLANTA2V_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_1/camada_12_planta3", 4, 9, 80, 80, TS_PLANTA3V, TS_PLANTA3V_F, TS_FRAME_TIME);

	TileMap::SetMapLayer(*this, "assets/tilemap/fase_1/camada_13_runa", 4, 9, 80, 80, TS_RUNA, TS_RUNA_W, TS_RUNA_H);

	//TileMap::SetMapLayer(*this, "assets/tilemap/fase_1/camada_16_pilar", 4, 9, 80, 80, TS_PILAR, TS_PILAR_W, TS_PILAR_H);

	TileMap::SetMapLayer(*this, "assets/tilemap/fase_1/camada_17_luz", 4, 9, 80, 80, TS_LUZ, TS_LUZ_W, TS_LUZ_H);
	// tmpmap->SetMapLayerRune(*this, "assets/tilemap/fase_1/camada_13_runa", 4, 9, 80, 80);

	MapColision::GetInstance().SetMapColision("assets/tilemap/fase_1/camada_18_colisao", 4, 9, 80, 80);
	//TileMap::SetMapLayer(*this, "assets/tilemap/fase_1/camada_18_colisao", 4, 9, 80, 80, TS_DEBUG, TS_DEBUG_W, TS_DEBUG_H);

	//EVENTS
	GameObject *goeve = new GameObject();
	weak_ptr = AddObject(goeve);
	ptr = weak_ptr.lock();
	Event* event = new Event(*ptr, 2720, 2440, 540, 471);
	ptr->AddComponent(event);

	goeve = new GameObject();
	weak_ptr = AddObject(goeve);
	ptr = weak_ptr.lock();
	MusicEvent* eventm = new MusicEvent(*ptr, 1161, 1514, 368, 590, "assets/audio/musica/battle_music.ogg");
	ptr->AddComponent(eventm);

	goeve = new GameObject();
	weak_ptr = AddObject(goeve);
	ptr = weak_ptr.lock();
	event = new Event(*ptr, 2720, 2440, 540, 471);
	ptr->AddComponent(event);

	goeve = new GameObject();
	weak_ptr = AddObject(goeve);
	ptr = weak_ptr.lock();
	event = new Event(*ptr, 2720, 2440, 540, 471);
	ptr->AddComponent(event);

	goeve = new GameObject();
	weak_ptr = AddObject(goeve);
	ptr = weak_ptr.lock();
	event = new Event(*ptr, 2030, 2745, 490, 355);
	ptr->AddComponent(event);

	goeve = new GameObject();
	weak_ptr = AddObject(goeve);
	ptr = weak_ptr.lock();
	event = new Event(*ptr, 2030, 2745, 490, 355);
	ptr->AddComponent(event);

	goeve = new GameObject();
	weak_ptr = AddObject(goeve);
	ptr = weak_ptr.lock();
	event = new Event(*ptr, 330, 4215, 830, 525);
	ptr->AddComponent(event);

	goeve = new GameObject();
	weak_ptr = AddObject(goeve);
	ptr = weak_ptr.lock();
	event = new Event(*ptr, 330, 4215, 830, 525);
	ptr->AddComponent(event);

	goeve = new GameObject();
	weak_ptr = AddObject(goeve);
	ptr = weak_ptr.lock();
	event = new Event(*ptr, 330, 4215, 830, 525);
	ptr->AddComponent(event);

	goeve = new GameObject();
	weak_ptr = AddObject(goeve);
	ptr = weak_ptr.lock();
	event = new Event(*ptr, 330, 4215, 830, 525);
	ptr->AddComponent(event);

	goeve = new GameObject();
	weak_ptr = AddObject(goeve);
	ptr = weak_ptr.lock();
	event = new Event(*ptr, 330, 4215, 830, 525);
	ptr->AddComponent(event);

	//PILAR
	GameObject *gopil = new GameObject();
	weak_ptr = AddObject(gopil);
	ptr = weak_ptr.lock();
	Sprite* pil = new Sprite(*ptr, "assets/img/tileset/pilar_bloqueio.png");
	ptr->AddComponent(pil);
	ptr->box.Centered({480,5367});

	gopil = new GameObject();
	weak_ptr = AddObject(gopil);
	ptr = weak_ptr.lock();
	pil = new Sprite(*ptr, "assets/img/tileset/pilar_bloqueio.png");
	ptr->AddComponent(pil);
	ptr->box.Centered({523,5342});

	gopil = new GameObject();
	weak_ptr = AddObject(gopil);
	ptr = weak_ptr.lock();
	pil = new Sprite(*ptr, "assets/img/tileset/pilar_bloqueio.png");
	ptr->AddComponent(pil);
	ptr->box.Centered({578,5310});

	gopil = new GameObject();
	weak_ptr = AddObject(gopil);
	ptr = weak_ptr.lock();
	pil = new Sprite(*ptr, "assets/img/tileset/pilar_bloqueio.png");
	ptr->AddComponent(pil);
	ptr->box.Centered({2107,3715});

	gopil = new GameObject();
	weak_ptr = AddObject(gopil);
	ptr = weak_ptr.lock();
	pil = new Sprite(*ptr, "assets/img/tileset/pilar_bloqueio.png");
	ptr->AddComponent(pil);
	ptr->box.Centered({2165,3715});

	gopil = new GameObject();
	weak_ptr = AddObject(gopil);
	ptr = weak_ptr.lock();
	pil = new Sprite(*ptr, "assets/img/tileset/pilar_bloqueio.png");
	ptr->AddComponent(pil);
	ptr->box.Centered({2225,3715});

	gopil = new GameObject();
	weak_ptr = AddObject(gopil);
	ptr = weak_ptr.lock();
	pil = new Sprite(*ptr, "assets/img/tileset/pilar_bloqueio.png");
	ptr->AddComponent(pil);
	ptr->box.Centered({2300,3715});

	// Capelobo Shadow
	GameObject* gosh = new GameObject();
	std::weak_ptr<GameObject> capsh_ptr = AddObject(gosh);
	ptr = capsh_ptr.lock();
	Sprite* capsh = new Sprite(*ptr, "assets/img/capelobo/sombra_capelobo.png");
	ptr->AddComponent(capsh);

	// Capelobo
	GameObject *goali1 = new GameObject();
	weak_ptr = AddObject(goali1);
	ptr = weak_ptr.lock();
	Capelobo *cape = new Capelobo(*ptr, capsh_ptr, 0.1);
	ptr->box.Centered({840, 1845});
	ptr->AddComponent(cape);

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
	ptr->box.Centered({510, 716});
	ptr->AddComponent(yawara);

	// Cursor
	GameObject *gocur = new GameObject();
	weak_ptr = AddObject(gocur);
	ptr = weak_ptr.lock();
	Cursor *cur = new Cursor(*ptr);
	Sprite *spcur = new Sprite(*ptr, STAGE_STT_CURSOR_SPRITE);
	spcur->SetScale(1, 1);
	ptr->AddComponent(cur);
	ptr->AddComponent(spcur);

	Camera::Follow(GetObjectPtr(goya), weak_ptr);
	Camera::ratio = STAGE_STT_CAMERA_RATIO;

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

	// BGM
	bgMusic.Open(STAGE_STT_BGM);
	bgMusic.Play(-1, 30);
}

StageState::~StageState()
{
	objectArray.clear();
}

void StageState::Update(float dt)
{
	static Timer fadein;

	if (black && flag) {
		fadein.Update(dt);
		
		black->SetAlphaMod(255 * (1 - QuadraticEaseIn(fadein.Get() / STAGE_STT_FADE)));

		if (fadein.Get() > STAGE_STT_FADE) {
			black->SetAlphaMod(0);
			fadein.Restart();
			flag = false;
		}
	}

	InputManager &input = InputManager::GetInstance();

	// verifica fechamento do jogo.
	if (input.KeyPress(ESCAPE_KEY))
	{
		PauseState *stage = new PauseState();
		Game::GetInstance().Push(stage);
	}

	if(input.QuitRequested()){
		quitRequested = true;
	}


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
			
			white->SetAlphaMod(255 * QuadraticEaseIn(fadein.Get() / STAGE_STT_FADE));

			if (fadein.Get() > STAGE_STT_FADE) {
				Data::pos = Yawara::player->GetCenterPos();

				popRequested = true;
				//Data::playerVictory = true;
				Phase2State *stage = new Phase2State();
				Game::GetInstance().Push(stage);
				ignore = true;
			}
		}

	}

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

void StageState::Render()
{
	RenderArray();
}

void StageState::LoadAssets()
{
}

void StageState::Start()
{
	LoadAssets();

	StartArray();

	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;
	
	// Black screen;
	GameObject* go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	black = new Sprite(*ptr, STAGE_STT_BLACK);
	CameraFollower* cmfr = new CameraFollower(*ptr);
	ptr->AddComponent(black);
	ptr->AddComponent(cmfr);

	// White screen;
	go = new GameObject();
	weak_ptr = AddObject(go);
	ptr = weak_ptr.lock();
	white = new Sprite(*ptr, STAGE_STT_WHITE);
	cmfr = new CameraFollower(*ptr);
	ptr->AddComponent(white);
	ptr->AddComponent(cmfr);
	white->SetAlphaMod(0);

	//Runas
	GameObject* gorune = new GameObject();
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
	
	gorune = new GameObject();
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
	DefenseRune* defrune = new DefenseRune(*ptr, 1.2);
	ptr->box.Centered(1382, 2828);
	ptr->AddComponent(defrune);

	gorune = new GameObject();
	weak_ptr = AddObject(gorune);
	ptr = weak_ptr.lock();
	defrune = new DefenseRune(*ptr, 1.3);
	ptr->box.Centered(1425, 4622);
	ptr->AddComponent(defrune);

	gorune = new GameObject();
	weak_ptr = AddObject(gorune);
	ptr = weak_ptr.lock();
	defrune = new DefenseRune(*ptr, 1.4);
	ptr->box.Centered(3651, 4605);
	ptr->AddComponent(defrune);

	gorune = new GameObject();
	weak_ptr = AddObject(gorune);
	ptr = weak_ptr.lock();
	defrune = new DefenseRune(*ptr, 1.15);
	ptr->box.Centered(3039, 5016);
	ptr->AddComponent(defrune);

	gorune = new GameObject();
	weak_ptr = AddObject(gorune);
	ptr = weak_ptr.lock();
	defrune = new DefenseRune(*ptr, 1.25);
	ptr->box.Centered(4751, 3285);
	ptr->AddComponent(defrune);

	gorune = new GameObject();
	weak_ptr = AddObject(gorune);
	ptr = weak_ptr.lock();
	defrune = new DefenseRune(*ptr, 1.5);
	ptr->box.Centered(1871, 8095);
	ptr->AddComponent(defrune);

	gorune = new GameObject();
	weak_ptr = AddObject(gorune);
	ptr = weak_ptr.lock();
	defrune = new DefenseRune(*ptr, 1.36);
	ptr->box.Centered(1773, 9245);
	ptr->AddComponent(defrune);

	ignore = false;
	flag = true;
	started = true;
}

void StageState::Pause()
{
}

void StageState::Resume()
{
}