#include "TileMap.h"
#include "Camera.h"

TileMap::TileMap(GameObject& associated, std::string file, TileSet* tileSet): Component(associated) {
	this->tileSet = tileSet;
	this->parallax = 0;
	Load(file);
}

void TileMap::Load(std::string file){
	std::ifstream input;
	std::string in;
	input.open(file.c_str(), std::ios::in);

	if (!input.fail()) {
		std::getline(input, in, ',');
		mapWidth = std::stoi(in);
		std::getline(input, in, ',');
		mapHeight = std::stoi(in);
		std::getline(input, in, ',');
		mapDepth = std::stoi(in);

		std::getline(input, in);
		for(int k = 0; k < mapDepth; k++){
			std::getline(input, in);
			for(int i = 0; i < mapHeight; i++){
				for(int j = 0; j < mapWidth; j++){
					std::getline(input, in, ',');
					tileMatrix.push_back(std::stoi(in) - 1);
				}
				std::getline(input, in);
			}
		}
	} else {
		std::cout << "Falha em carregar tilemap: " << file.c_str() << "\n";
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
			tileSet->RenderTile(tileMatrix[j + i*mapWidth + layer*mapWidth*mapHeight], (j+1)*tileSet->GetTileWidth() - cameraX, (i+0.5)*tileSet->GetTileHeight() - cameraY);
		}
	}
}

void TileMap::Render(){
	for(int i = 0; i < mapDepth; i++){
		RenderLayer(i, Camera::pos.x + (int)Camera::pos.x*parallax*i, Camera::pos.y + (int)Camera::pos.y*parallax*i);
	}
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

void TileMap::Update(float dt) {

}

bool TileMap::Is(std::string type) {
	return !strcmp(type.c_str(), "TileMap");
}