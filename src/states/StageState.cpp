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

#include "Tilesets.h"
#include "Tilemaps.h"

#define STAGE_STT_BG "assets/penguin/img/ocean.jpg"
#define STAGE_STT_BGM "assets/audio/musica/tema_triste.ogg"
#define STAGE_STT_CURSOR_SPRITE "assets/img/cursor/cursor.png"
#define STAGE_STT_CAMERA_RATIO 0.37

StageState::StageState()
{
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

	TileMap::SetMapLayer(*this, "assets/tilemap/fase_1/camada_16_pilar", 4, 9, 80, 80, TS_PILAR, TS_PILAR_W, TS_PILAR_H);

	TileMap::SetMapLayer(*this, "assets/tilemap/fase_1/camada_17_luz", 4, 9, 80, 80, TS_LUZ, TS_LUZ_W, TS_LUZ_H);

	MapColision::GetInstance().SetMapColision("assets/tilemap/fase_1/camada_18_colisao", 4, 9, 80, 80);
	//TileMap::SetMapLayer(*this, "assets/tilemap/fase_1/camada_18_colisao", 4, 9, 80, 80, TS_DEBUG, TS_DEBUG_W, TS_DEBUG_H);


	/* // Fase2
	TileMap::SetMapLayer(*this, "assets/tilemap/fase_2/camada_1_solo", 4, 9, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/fase_2/camada_2_solo", 4, 9, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/fase_2/camada_3_solo", 4, 9, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/fase_2/camada_4_solo", 4, 9, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);
	TileMap::SetMapLayer(*this, "assets/tilemap/fase_2/camada_5_solo", 4, 9, 80, 80, TS_SOLO, TS_SOLO_W, TS_SOLO_H);

	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_2/camada_6_arvorevoid3", 4, 9, 80, 80, TS_ARVORE3V, TS_ARVORE3V_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_2/camada_7_arvorevoid2", 4, 9, 80, 80, TS_ARVORE2V, TS_ARVORE2V_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_2/camada_8_arvorevoid1", 4, 9, 80, 80, TS_ARVORE1V, TS_ARVORE1V_F, TS_FRAME_TIME);

	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_2/camada_9_arvorecerrado2", 4, 9, 80, 80, TS_ARVORE2C, TS_ARVORE2C_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_2/camada_10_arvorecerrado3", 4, 9, 80, 80, TS_ARVORE3C, TS_ARVORE3C_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_2/camada_11_arvorecerrado1", 4, 9, 80, 80, TS_ARVORE1C, TS_ARVORE1C_F, TS_FRAME_TIME);

	TileMap::SetMapLayer(*this, "assets/tilemap/fase_2/camada_12_pedra", 4, 9, 80, 80, TS_PEDRA, TS_PEDRA_W, TS_PEDRA_H);

	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_2/camada_13_planta1", 4, 9, 80, 80, TS_PLANTA1V, TS_PLANTA1V_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_2/camada_14_planta2", 4, 9, 80, 80, TS_PLANTA2V, TS_PLANTA2V_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_2/camada_15_planta3", 4, 9, 80, 80, TS_PLANTA3V, TS_PLANTA3V_F, TS_FRAME_TIME);

	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_2/camada_16_planta4", 4, 9, 80, 80, TS_PLANTA1C, TS_PLANTA1C_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_2/camada_17_planta5", 4, 9, 80, 80, TS_PLANTA2C, TS_PLANTA2C_F, TS_FRAME_TIME);
	tmpmap->SetMapLayerAnimation(*this, "assets/tilemap/fase_2/camada_18_planta6", 4, 9, 80, 80, TS_PLANTA3C, TS_PLANTA3C_F, TS_FRAME_TIME);

	TileMap::SetMapLayer(*this, "assets/tilemap/fase_2/camada_19_runa", 4, 9, 80, 80, TS_RUNA, TS_RUNA_W, TS_RUNA_H);

	TileMap::SetMapLayer(*this, "assets/tilemap/fase_2/camada_22_pilar", 4, 9, 80, 80, TS_PILAR, TS_PILAR_W, TS_PILAR_H);

	TileMap::SetMapLayer(*this, "assets/tilemap/fase_2/camada_23_luz", 4, 9, 80, 80, TS_LUZ, TS_LUZ_W, TS_LUZ_H);

	MapColision::GetInstance().SetMapColision("assets/tilemap/fase_2/camada_24_colisao", 4, 9, 80, 80);
	//TileMap::SetMapLayer(*this, "assets/tilemap/fase_2/camada_24_colisao", 4, 9, 80, 80, TS_DEBUG, TS_DEBUG_W, TS_DEBUG_H);
	*/

	/* // Fase3
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

	/*
	// Capelobo
	GameObject *goali1 = new GameObject();
	weak_ptr = AddObject(goali1);
	ptr = weak_ptr.lock();
	Capelobo *cape = new Capelobo(*ptr, 0.1);
	ptr->box.Centered({512, 850});
	ptr->AddComponent(cape);
	*/

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
	ptr->box.Centered({1150, 30});
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