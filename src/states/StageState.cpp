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
#include "Floor.h"
#include "HealthRune.h"
#include "AttackRune.h"
#include "DefenseRune.h"

#include "Tilesets.h"
#include "Tilemaps.h"

#define STAGE_STT_BG "assets/penguin/img/ocean.jpg"
#define STAGE_STT_BGM "assets/audio/musica/tema_triste.ogg"//"assets/penguin/audio/stageState.ogg"
#define STAGE_STT_CURSOR_SPRITE "assets/penguin/img/penguinface.png"
#define STAGE_STT_CAMERA_RATIO 0.37

StageState::StageState()
{
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;

	Floor::Load(CL_MAP1, TILE, TILE);

	// Background
	GameObject *gobg = new GameObject();
	weak_ptr = AddObject(gobg);
	ptr = weak_ptr.lock();
	ptr->box.x = 0;
	ptr->box.y = 0;
	Sprite *sp = new Sprite(*ptr, STAGE_STT_BG);
	CameraFollower *cmfl = new CameraFollower(*ptr);
	ptr->AddComponent(sp);
	ptr->AddComponent(cmfl);

	// TileMap
	TileMap::SetMapLayer(*this, "assets/tilemap/test/dark/camada_1_solo", 4, 8, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/test/dark/camada_2_solo", 4, 8, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/test/dark/camada_3_solo", 4, 8, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/test/dark/camada_4_solo", 4, 8, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/test/dark/camada_5_solo", 4, 8, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	/*
	GameObject *gomp1 = new GameObject();
	weak_ptr = AddObject(gomp1);
	ptr = weak_ptr.lock();
	tileset = new TileSet(*ptr, TS_SOLO_W, TS_SOLO_H, TS_SOLO);
	TileMap *tlmp1 = new TileMap(*ptr, TILE, TILE, TM_MAP1_1, tileset);
	tlmp1->SetParallax(1);
	ptr->box.x = 0;
	ptr->box.y = 0;
	ptr->AddComponent(tlmp1);

	GameObject *gomp2 = new GameObject();
	weak_ptr = AddObject(gomp2);
	ptr = weak_ptr.lock();
	tileset = new TileSet(*ptr, TS_SOLO_W, TS_SOLO_H, TS_SOLO);
	TileMap *tlmp2 = new TileMap(*ptr, TILE, TILE, TM_MAP1_2, tileset);
	tlmp2->SetParallax(1);
	ptr->box.x = 0;
	ptr->box.y = 0;
	ptr->AddComponent(tlmp2);

	GameObject *gomp3 = new GameObject();
	weak_ptr = AddObject(gomp3);
	ptr = weak_ptr.lock();
	tileset = new TileSet(*ptr, TS_SOLO_W, TS_SOLO_H, TS_SOLO);
	TileMap *tlmp3 = new TileMap(*ptr, TILE, TILE, TM_MAP1_3, tileset);
	// TileMap *tlmp3 = new TileMap(*ptr, TILE, TILE, CL_MAP1, tileset);
	tlmp3->SetParallax(1);
	ptr->box.x = 0;
	ptr->box.y = 0;
	ptr->AddComponent(tlmp3);

	GameObject *gomp4 = new GameObject();
	weak_ptr = AddObject(gomp4);
	ptr = weak_ptr.lock();
	tileset = new TileSet(*ptr, TS_TREE_W, TS_TREE_H, TS_TREE);
	TileMap *tlmp4 = new TileMap(*ptr, TILE, TILE, TM_MAP1_4, tileset);
	tlmp4->SetParallax(1);
	ptr->box.x = 0;
	ptr->box.y = 0;
	ptr->AddComponent(tlmp4);

	GameObject *gomp5 = new GameObject();
	weak_ptr = AddObject(gomp5);
	ptr = weak_ptr.lock();
	tileset = new TileSet(*ptr, TS_PLANT_ROCK_W, TS_PLANT_ROCK_H, TS_PLANT_ROCK);
	TileMap *tlmp5 = new TileMap(*ptr, TILE, TILE, TM_MAP1_5, tileset);
	tlmp5->SetParallax(1);
	ptr->box.x = 0;
	ptr->box.y = 0;
	ptr->AddComponent(tlmp5);
	*/
	//Runas
	GameObject *gorune = new GameObject();
	weak_ptr = AddObject(gorune);
	ptr = weak_ptr.lock();
	HealthRune *rune1 = new HealthRune(*ptr, 1.5);
	ptr->box.Centered(550,1200);
	ptr->AddComponent(rune1);

	GameObject *gorune2 = new GameObject();
	weak_ptr = AddObject(gorune2);
	ptr = weak_ptr.lock();
	AttackRune *rune2 = new AttackRune(*ptr, 1.5);
	ptr->box.Centered(1000,850);
	ptr->AddComponent(rune2);
	
	GameObject *gorune3 = new GameObject();
	weak_ptr = AddObject(gorune3);
	ptr = weak_ptr.lock();
	DefenseRune *rune3 = new DefenseRune(*ptr, 1.5);
	ptr->box.Centered(1300,500);
	ptr->AddComponent(rune3);

	// Capelobo
	GameObject *goali1 = new GameObject();
	weak_ptr = AddObject(goali1);
	ptr = weak_ptr.lock();
	Capelobo *cape = new Capelobo(*ptr, 0.1);
	ptr->box.Centered({512, 850});
	ptr->AddComponent(cape);

	// Yawara
	GameObject *goya = new GameObject();
	weak_ptr = AddObject(goya);
	ptr = weak_ptr.lock();
	Yawara *yawara = new Yawara(*ptr);
	ptr->box.Centered({1150, 30});
	ptr->AddComponent(yawara);

	// Cursor
	GameObject *gocur = new GameObject();
	weak_ptr = AddObject(gocur);
	ptr = weak_ptr.lock();
	Cursor *cur = new Cursor(*ptr);
	Sprite *spcur = new Sprite(*ptr, STAGE_STT_CURSOR_SPRITE);
	spcur->SetScale(0.1, 0.1);
	ptr->AddComponent(cur);
	ptr->AddComponent(spcur);

	Camera::Follow(GetObjectPtr(goya), weak_ptr);
	Camera::ratio = STAGE_STT_CAMERA_RATIO;

	// BGM
	bgMusic.Open(STAGE_STT_BGM);
	bgMusic.Play();
}

StageState::~StageState()
{
	objectArray.clear();
}

void StageState::Update(float dt)
{
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
	if (Yawara::player == nullptr)
	{
		popRequested = true;
		Data::playerVictory = false;
		EndState *stage = new EndState();
		Game::GetInstance().Push(stage);
	}
	/*else if (Capelobo::boss == nullptr)
	{
		popRequested = true;
		Data::playerVictory = true;
		EndState *stage = new EndState();
		Game::GetInstance().Push(stage);
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

	started = true;
}

void StageState::Pause()
{
}

void StageState::Resume()
{
}