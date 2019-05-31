#pragma once // Alows to initializate the header just once

#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

class Timer
{
private:
	float time;

public:
	Timer();

	void Update(float);
	void Restart();
	float Get();
};