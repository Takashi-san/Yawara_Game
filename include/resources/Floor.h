#pragma once // Alows to initializate the header just once

#include "Camera.h"
#include <vector>
#include <fstream>
#include <cstring>

using namespace std;

class Floor
{
private:
    static vector<int> floorMatrix;
    static int mapWidth;
    static int mapHeight;
    static int tileWidth;
    static int tileHeight;

public:
    static bool loaded;
    static Vec2 pos;
    static Vec2 speed;
    static float ratio;

    static void Load(string, int, int);
    static bool AtAllowedArea(int, int, int);

    static void Update(float);
    static void Render();
    static bool Is(string);

    static int GetWidth();
    static int GetHeight();

    static vector<int> GetMatrix();
};