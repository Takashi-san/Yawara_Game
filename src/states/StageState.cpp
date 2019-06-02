#include "StageState.h"

#include "Sprite.h"
#include "Sound.h"
#include "TileMap.h"
#include "Vec2.h"
#include "InputManager.h"
#include "Camera.h"
#include "CameraFollower.h"
#include "Alien.h"
#include "Collision.h"
#include "Collider.h"
#include "Data.h"
#include "EndState.h"
#include "Game.h"
#include "Cursor.h"
#include "Yawara.h"

#include "Tilesets.h"
#include "Tilemaps.h"

#define STAGE_STT_BG			"assets/penguin/img/ocean.jpg"
#define STAGE_STT_BGM			"assets/penguin/audio/stageState.ogg"
#define STAGE_STT_CURSOR_SPRITE	"assets/penguin/img/penguinface.png"
#define STAGE_STT_CAMERA_RATIO	0.37

StageState::StageState() {
	std::weak_ptr<GameObject> weak_ptr;
	std::shared_ptr<GameObject> ptr;

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

	// Alien
	GameObject *goali1 = new GameObject();
	weak_ptr = AddObject(goali1);
	ptr = weak_ptr.lock();
	Alien *alien1 = new Alien(*ptr, 5, 0.5);
	ptr->box.x = 512 - goali1->box.w/2;
	ptr->box.y = 300 - goali1->box.h/2;
	ptr->AddComponent(alien1);

	// Yawara
	GameObject *goya = new GameObject();
	weak_ptr = AddObject(goya);
	ptr = weak_ptr.lock();
	Yawara *yawara = new Yawara(*ptr);
	ptr->box.Centered({0, 0});
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

StageState::~StageState() {
	objectArray.clear();
}

void StageState::Update(float dt) {
	InputManager& input = InputManager::GetInstance();

	// verifica fechamento do jogo.
	if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
		popRequested = true;
	}

	// verifica condições de vitoria.
	if (Yawara::player == nullptr) {
		popRequested = true;
		Data::playerVictory = false;
		EndState *stage = new EndState();
		Game::GetInstance().Push(stage);
	} else if (Alien::alienCount == 0) {
		popRequested = true;
		Data::playerVictory = true;
		EndState *stage = new EndState();
		Game::GetInstance().Push(stage);
	}
	
	UpdateArray(dt);

	// update dos colliders.
	for (unsigned int i = 0; i < objectArray.size(); i++) {
		Collider *coli = static_cast<Collider*>(objectArray[i]->GetComponent("Collider"));
		if (coli != nullptr) {
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
	for (unsigned int i = 0; i < objectArray.size(); i++) {
		Collider *coli = static_cast<Collider*>(objectArray[i]->GetComponent("CameraFollower"));
		if (coli != nullptr) {
			coli->Update(dt);
			break;
		}
	}

	// update dos Cursor.
	for (unsigned int i = 0; i < objectArray.size(); i++) {
		Collider *coli = static_cast<Collider*>(objectArray[i]->GetComponent("Cursor"));
		if (coli != nullptr) {
			coli->Update(dt);
			break;
		}
	}
}

void StageState::Render() {
	RenderArray();
}

void StageState::LoadAssets() {

}

void StageState::Start(){
	LoadAssets();
	
	StartArray();

	started = true;
}

void StageState::Pause() {

}

void StageState::Resume() {

}