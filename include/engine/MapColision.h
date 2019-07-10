#pragma once // Alows to initializate the header just once

#include "Vec2.h"
#include "Rect.h"

#include <string>
#include <iostream>
#include <stdbool.h>
#include <vector>
#include <fstream>

class MapColision
{
private:
	std::vector< std::vector<int> > colisionMatrix;
	bool load_fail;
	
	int pos_x, pos_y;
	int mapSize_x, mapSize_y;
	int matrixSize_x, matrixSize_y;

	MapColision();
	~MapColision();

public:
	void Load(std::string);
	void SetMapColision (std::string, int, int, int, int, int = 0, int = 0);
	
	bool OutOfBounds(int, int);

	// position.
	bool Validate(int, int);
	bool Validate(Vec2);

	// area.
	bool Validate(int, int, int, int);
	bool Validate(Rect);

	// position + velocity.
	Vec2 Validate(int, int, Vec2, float);
	Vec2 Validate(Vec2, Vec2, float);

	// area + velocity.
	Vec2 Validate(int, int, int, int, Vec2, float);
	Vec2 Validate(Rect, Vec2, float);

	static MapColision& GetInstance();
};