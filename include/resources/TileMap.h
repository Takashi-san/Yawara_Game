#include "Component.h"
#include "TileSet.h"
#include "GameObject.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <vector>
#include <fstream>

#ifndef TILEMAP
	#define TILEMAP

	class TileMap : public Component{
	private:
		std::vector<int> tileMatrix;
		TileSet* tileSet;
		int mapWidth, mapHeight, mapDepth;
		float parallax;

	public:
		TileMap(GameObject&, std::string, TileSet*);

		void Load(std::string);
		void SetTileSet(TileSet*);
		int& At(int, int, int);
		void Render();
		void RenderLayer(int, int, int);
		int GetWidth();
		int GetHeight();
		int GetDepth();
		void SetParallax(float);

		void Update(float dt);
		bool Is(std::string type);
	};

#endif
