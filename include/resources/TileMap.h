#pragma once // Alows to initializate the header just once

#include "Component.h"
#include "TileSet.h"
#include "GameObject.h"
#include "State.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <vector>
#include <fstream>

class TileMap : public Component
{
private:
	std::vector<int> tileMatrix;
	TileSet *tileSet;
	int mapWidth, mapHeight, mapDepth;
	int tileWidth, tileHeight;
	float parallax;

public:
	TileMap(GameObject&);
	TileMap(GameObject &, int, int, std::string, TileSet *);

	void Load(std::string);
	void SetTileSet(TileSet *);
	int &At(int, int, int);
	void Render();
	void RenderLayer(int, int, int);

	int GetWidth();
	int GetHeight();
	int GetDepth();

	void SetParallax(float);
	void SetTileWidth(int);
	void SetTileHeight(int);

	void Update(float dt);
	bool Is(std::string type);

	static void SetMapLayer(State&, std::string, int, int, int, int, std::string, int, int, int = 0, int = 0);
	void SetMapLayerAnimation(State&, std::string, int, int, int, int, std::string, int, float, int = 0, int = 0);
};
