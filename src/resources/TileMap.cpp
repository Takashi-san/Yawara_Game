#include "TileMap.h"
#include "Camera.h"
#include "Tilemaps.h"
#include "Tilesets.h"
#include "Game.h"
#include "TileSet.h"
#include <sstream>

TileMap::TileMap(GameObject& associated, int tileWidth, int tileHeight, std::string file, TileSet* tileSet): Component(associated) {
	this->tileSet = tileSet;
	this->parallax = 0;
	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;
	Load(file);
}

void TileMap::Load(std::string file){
	mapWidth = 0;
	mapHeight = 0;
	mapDepth = 1;
	int count = 0;
	std::ifstream input;
	std::string in, num;
	input.open(file.c_str(), std::ios::in);

	if (!input.fail()) {
		while (!input.eof()) {
			std::getline(input, in);
			if (in.size() != 0) {
				mapHeight++;
				std::stringstream line(in);
				count = 0;
				while (!line.eof()) {
					count++;
					std::getline(line, num, ',');
					tileMatrix.push_back(std::stoi(num));
				}
				mapWidth = count;
			}
		}
	} else {
		#ifdef DEBUG
		std::cout << "Falha em carregar tilemap: " << file.c_str() << "\n";
		#endif
	}

	input.close();
}

void TileMap::SetTileSet(TileSet* tileSet) {
	this->tileSet = tileSet;
}

int& TileMap::At(int x, int y, int z=0) {
	return tileMatrix[x + y*mapWidth + z*mapWidth*mapHeight];
}

void TileMap::RenderLayer(int layer, int cameraX=0, int cameraY=0) {
	for(int i = 0; i < mapHeight; i++){
		for(int j = 0; j < mapWidth; j++){
			tileSet->RenderTile(tileMatrix[j + i*mapWidth + layer*mapWidth*mapHeight], j*tileWidth - cameraX + associated.box.x, (i+1)*tileHeight - tileSet->GetTileHeight() - cameraY + associated.box.y);
		}
	}
}

void TileMap::Render(){
	Game& instance = Game::GetInstance();
	int x = associated.box.x - Camera::pos.x;
	int y = associated.box.y - Camera::pos.y;

	// Renderiza apenas o que pode ser visível na tela.
	Vec2 tmp(mapWidth*tileWidth, mapHeight*tileHeight);
	if ((x < -tmp.x) || (y < -tmp.y)) {
		return;
	} else if ((x > instance.GetWindowSize().x) || (y > instance.GetWindowSize().y)) {
		return;
	}

	RenderLayer(0, Camera::pos.x, Camera::pos.y);
}

int TileMap::GetWidth() {
	return mapWidth;
}

int TileMap::GetHeight() {
	return mapHeight;
}

int TileMap::GetDepth() {
	return mapDepth;
}

void TileMap::SetParallax(float parallax) {
	this->parallax = parallax;
}

void TileMap::SetTileHeight(int tileHeight) {
	this->tileHeight = tileHeight;
}

void TileMap::SetTileWidth(int tileWidth) {
	this->tileWidth = tileWidth;
}

void TileMap::Update(float dt) {

}

bool TileMap::Is(std::string type) {
	return !strcmp(type.c_str(), "TileMap");
}

void TileMap::SetMapLayer(State& state, std::string layer_path, int size_x, int size_y, int csv_x, int csv_y, std::string tileset_path, int tileset_w, int tileset_h, int offsetX, int offsetY) {
	// setup tileset.
	GameObject* go = new GameObject();
	std::weak_ptr<GameObject> weak_ptr = state.AddObject(go);
	std::shared_ptr<GameObject> ptr = weak_ptr.lock();
	TileSet* tileset = new TileSet(*ptr, tileset_w, tileset_h, tileset_path);

	std::string tilemap;

	for(int y = 1; y <= size_y; y++) {
		for(int x = 1; x <= size_x; x++) {
			go = new GameObject();
			weak_ptr = state.AddObject(go);
			ptr = weak_ptr.lock();

			tilemap = layer_path;
			tilemap += "/x";
			tilemap += std::to_string(x);
			tilemap += "_y";
			tilemap += std::to_string(y);
			tilemap += ".csv";

			TileMap* tlmp = new TileMap(*ptr, TILE, TILE, tilemap, tileset);
			ptr->box.x = offsetX + csv_x*tileset_w*(x-1);
			ptr->box.y = offsetY + csv_y*tileset_h*(y-1);
			ptr->AddComponent(tlmp);
		}
	}
}