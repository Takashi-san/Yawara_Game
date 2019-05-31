#pragma once // Alows to initializate the header just once

#include "Component.h"
#include "Camera.h"
#include "PenguinBody.h"
#include <vector>
#include <fstream>
#include <cstring>

using namespace std;

class Floor : public Component
{
private:
    vector<int> floorMatrix;
    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;

public:
    Floor(GameObject &, int, int, string);
    ~Floor();

    void Load(std::string);
    bool AtAllowedArea(int, int, int);

    void Update(float dt);
    void Render();
    bool Is(string type);

    vector<int> GetMatrix();
};