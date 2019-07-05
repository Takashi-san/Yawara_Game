#include "Floor.h"

std::vector<int> Floor::floorMatrix;
int Floor::mapWidth;
int Floor::mapHeight;
int Floor::tileWidth;
int Floor::tileHeight;
bool Floor::loaded = false;
Vec2 Floor::pos;
Vec2 Floor::speed;
float Floor::ratio;

void Floor::Load(std::string file, int tileWidth, int tileHeight)
{
    Floor::tileWidth = tileWidth;
    Floor::tileHeight = tileHeight;
    std::ifstream input;
    std::string in;
    input.open(file.c_str(), std::ios::in);

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
        std::cout << "Fail to load floor: " << file.c_str() << "\n";
    }

    input.close();
    loaded = true;
}

bool Floor::AtAllowedArea(int x, int y, int z = 0)
{
    if (x < 0 || y < 0 || x > mapWidth * Floor::tileWidth || y > mapHeight * Floor::tileHeight)
        return false;
    return floorMatrix[x / Floor::tileWidth + y / Floor::tileHeight * mapWidth + z * mapWidth * mapHeight];
}

void Floor::Update(float dt)
{
    pos = Camera::pos;
    speed = Camera::speed;
    ratio = Camera::ratio;
}

void Floor::Render()
{
}

bool Floor::Is(std::string type)
{
    return !strcmp(type.c_str(), "Floor");
}

int Floor::GetWidth()
{
    return tileWidth;
}

int Floor::GetHeight()
{
    return tileHeight;
}

std::vector<int> Floor::GetMatrix()
{
    return floorMatrix;
}