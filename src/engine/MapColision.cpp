#include "MapColision.h"
#include "Tilemaps.h"
#include <sstream>

MapColision& MapColision::GetInstance(){
	static MapColision instance;
	return instance;
}

MapColision::MapColision() {
	load_fail = false;
	pos_x = 0;
	pos_y = 0;
	mapSize_x = 0;
	mapSize_y = 0;
	matrixSize_x = 0;
	matrixSize_y = 0;
}

MapColision::~MapColision() {
	colisionMatrix.clear();
}

void MapColision::Load(std::string file){
	std::ifstream input;
	std::string in, num;
	input.open(file.c_str(), std::ios::in);
	std::vector<int> colisionMap;

	if (!input.fail()) {
		while (!input.eof()) {
			std::getline(input, in);
			if (in.size() != 0) {
				std::stringstream line(in);
				while (!line.eof()) {
					std::getline(line, num, ',');
					colisionMap.push_back(std::stoi(num));
				}
			}
		}
	} else {
		std::cout << "Falha em carregar tilemap de colisão: " << file.c_str() << "\n";
		load_fail = true;
		return;
	}

	colisionMatrix.push_back(colisionMap);

	input.close();
}

void MapColision::SetMapColision (std::string matrix_path, int matrix_x, int matrix_y, int csv_x, int csv_y, int pos_x, int pos_y) {
	load_fail = false;
	this->pos_x = pos_x;
	this->pos_y = pos_y;
	mapSize_x = csv_x;
	mapSize_y = csv_y;
	matrixSize_x = matrix_x;
	matrixSize_y = matrix_y;

	std::string colision_map;

	for(int y = 1; y <= matrix_y; y++) {
		for(int x = 1; x <= matrix_x; x++) {
			colision_map = matrix_path;
			colision_map += "/x";
			colision_map += std::to_string(x);
			colision_map += "_y";
			colision_map += std::to_string(y);
			colision_map += ".csv";

			Load(colision_map);

			if (load_fail) {
				colisionMatrix.clear();
				return;
			}
		}
	}
}

bool MapColision::OutOfBounds(int x, int y) {
	if ((x < pos_x) || (y < pos_y)) {
		return true;
	} else if ((x >= pos_x + mapSize_x*matrixSize_x*TILE) || (y >= pos_y + mapSize_y*matrixSize_y*TILE)) {
		return true;
	}
	return false;
}

bool MapColision::Validate (int x, int y) {
	// Out of bounds.
	if (OutOfBounds(x, y)) {
		return true;
	}

	// Inside of map.
	int targetMatrix_x = (x - pos_x)/(mapSize_x*TILE);
	int targetMatrix_y = (y - pos_y)/(mapSize_y*TILE);
	int targetMap_x = ((x - pos_x) - targetMatrix_x*mapSize_x*TILE)/TILE;
	int targetMap_y = ((y - pos_y) - targetMatrix_y*mapSize_y*TILE)/TILE;

	return colisionMatrix[targetMatrix_x + targetMatrix_y*matrixSize_x][targetMap_x + targetMap_y*mapSize_x];
}

bool MapColision::Validate (Vec2 position) {
	return Validate((int)position.x, (int)position.y);
}

bool MapColision::Validate (int x, int y, int w, int h) {
	// Out of bounds.
	if ((OutOfBounds(x, y)) || (OutOfBounds(x + w, y)) || (OutOfBounds(x, y + h)) || (OutOfBounds(x + w, y + h))) {
		return true;
	}

	// Check the 4 corners.
	if ((!Validate(x, y)) || (!Validate(x + w, y)) || (!Validate(x, y + h)) || (!Validate(x + w, y + h))) {
		return false;
	}

	// Check all tiles that the area covers.
	int initial_targetMatrix_x = (x - pos_x)/(mapSize_x*TILE);
	int initial_targetMatrix_y = (y - pos_y)/(mapSize_y*TILE);
	int initial_targetMap_x = ((x - pos_x) - initial_targetMatrix_x*mapSize_x*TILE)/TILE;
	int initial_targetMap_y = ((y - pos_y) - initial_targetMatrix_y*mapSize_y*TILE)/TILE;

	int final_targetMatrix_x = ((x+w) - pos_x)/(mapSize_x*TILE);
	int final_targetMatrix_y = ((y+h) - pos_y)/(mapSize_y*TILE);
	int final_targetMap_x = (((x+w) - pos_x) - final_targetMatrix_x*mapSize_x*TILE)/TILE;
	int final_targetMap_y = (((y+h) - pos_y) - final_targetMatrix_y*mapSize_y*TILE)/TILE;

	// Matrix y.
	for (int matrix_y = initial_targetMatrix_y; matrix_y <= final_targetMatrix_y; matrix_y++) {
		
		// Map y.
		if (matrix_y == final_targetMatrix_y) {
			if (matrix_y == initial_targetMatrix_y) {
				for (int map_y = initial_targetMap_y; map_y <= final_targetMap_y; map_y++) {

					// Matrix x.
					for (int matrix_x = initial_targetMatrix_x; matrix_x <= final_targetMatrix_x; matrix_x++) {
						
						// Map x.
						if (matrix_x == final_targetMatrix_x) {
							if (matrix_x == initial_targetMatrix_x) {
								for (int map_x = initial_targetMap_x; map_x <= final_targetMap_x; map_x++) {
									
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							} else {
								for (int map_x = 0; map_x <= final_targetMap_x; map_x++) {
								
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							}
						} else {
							if (matrix_x == initial_targetMatrix_x) {
								for (int map_x = initial_targetMap_x; map_x < mapSize_x; map_x++) {
								
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							} else {
								for (int map_x = 0; map_x < mapSize_x; map_x++) {
								
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							}
						}
					}
				}
			} else {
				for (int map_y = 0; map_y <= final_targetMap_y; map_y++) {

					// Matrix x.
					for (int matrix_x = initial_targetMatrix_x; matrix_x <= final_targetMatrix_x; matrix_x++) {

						// Map x.
						if (matrix_x == final_targetMatrix_x) {
							if (matrix_x == initial_targetMatrix_x) {
								for (int map_x = initial_targetMap_x; map_x <= final_targetMap_x; map_x++) {
									
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							} else {
								for (int map_x = 0; map_x <= final_targetMap_x; map_x++) {
								
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							}
						} else {
							if (matrix_x == initial_targetMatrix_x) {
								for (int map_x = initial_targetMap_x; map_x < mapSize_x; map_x++) {
								
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							} else {
								for (int map_x = 0; map_x < mapSize_x; map_x++) {
								
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							}
						}
					}
				}
			}
		} else {
			if (matrix_y == initial_targetMatrix_y) {
				for (int map_y = initial_targetMap_y; map_y < mapSize_y; map_y++) {

					// Matrix x.
					for (int matrix_x = initial_targetMatrix_x; matrix_x <= final_targetMatrix_x; matrix_x++) {

						// Map x.
						if (matrix_x == final_targetMatrix_x) {
							if (matrix_x == initial_targetMatrix_x) {
								for (int map_x = initial_targetMap_x; map_x <= final_targetMap_x; map_x++) {
									
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							} else {
								for (int map_x = 0; map_x <= final_targetMap_x; map_x++) {
								
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							}
						} else {
							if (matrix_x == initial_targetMatrix_x) {
								for (int map_x = initial_targetMap_x; map_x < mapSize_x; map_x++) {
								
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							} else {
								for (int map_x = 0; map_x < mapSize_x; map_x++) {
								
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							}
						}
					}
				}
			} else {
				for (int map_y = 0; map_y < mapSize_y; map_y++) {

					// Matrix x.
					for (int matrix_x = initial_targetMatrix_x; matrix_x <= final_targetMatrix_x; matrix_x++) {

						// Map x.
						if (matrix_x == final_targetMatrix_x) {
							if (matrix_x == initial_targetMatrix_x) {
								for (int map_x = initial_targetMap_x; map_x <= final_targetMap_x; map_x++) {
									
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							} else {
								for (int map_x = 0; map_x <= final_targetMap_x; map_x++) {
								
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							}
						} else {
							if (matrix_x == initial_targetMatrix_x) {
								for (int map_x = initial_targetMap_x; map_x < mapSize_x; map_x++) {
								
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							} else {
								for (int map_x = 0; map_x < mapSize_x; map_x++) {
								
									// Validate.
									if (!colisionMatrix[matrix_x + matrix_y*matrixSize_x][map_x + map_y*mapSize_x]) {
										return false;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return true;
}

bool MapColision::Validate (Rect area) {
	return Validate((int)area.x, (int)area.y, (int)area.w, (int)area.h);
}