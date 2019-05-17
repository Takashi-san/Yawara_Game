#include "TileSet.h"

TileSet::TileSet(GameObject& associated, int tileWidth, int tileHeight, std::string file): tileSet(associated, file){
	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;

	if (tileSet.IsOpen()){
		columns = tileSet.GetWidth()/tileWidth;
		rows = tileSet.GetHeight()/tileHeight;
	} else {
		std::cout << "Falha em carregar tileset: " << file.c_str() << "\n";
		columns = 0;
		rows = 0;
	}
}

void TileSet::RenderTile(unsigned int index, int x, int y) {
	if (index < (unsigned int)rows*columns) {
		tileSet.SetClip((index%columns)*tileWidth, (index/columns)*tileHeight, tileWidth, tileHeight);
		tileSet.Render(x, y);
	}
}

int TileSet::GetTileWidth() {
	return tileWidth;
}

int TileSet::GetTileHeight() {
	return tileHeight;
}