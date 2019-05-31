#include "Floor.h"

Floor::Floor(GameObject &associated, int tileWidth, int tileHeight, string file) : Component(associated)
{
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    Load(file);
}

Floor::~Floor()
{
}

void Floor::Load(string file)
{
    ifstream input;
    string in;
    input.open(file.c_str(), ios::in);

    if (!input.fail())
    {
        getline(input, in, ',');
        mapWidth = stoi(in);
        getline(input, in, ',');
        mapHeight = stoi(in);
        getline(input, in, ',');

        getline(input, in);
        getline(input, in);
        for (int i = 0; i < mapHeight; i++)
        {
            for (int j = 0; j < mapWidth; j++)
            {
                getline(input, in, ',');
                floorMatrix.push_back(stoi(in));
            }
            getline(input, in);
        }
    }
    else
    {
        cout << "Fale to load floor: " << file.c_str() << "\n";
    }

    input.close();
}

bool Floor::AtAllowedArea(int x, int y, int z = 0)
{
    return floorMatrix[x + y * mapWidth + z * mapWidth * mapHeight];
}

void Floor::Update(float dt)
{
}

void Floor::Render()
{
}

bool Floor::Is(string type)
{
    return !strcmp(type.c_str(), "Floor");
}

vector<int> Floor::GetMatrix()
{
    return floorMatrix;
}