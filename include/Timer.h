#include <string>
#include <iostream>
#include <stdbool.h>
#include <memory>

#ifndef TIMER
	#define TIMER

	class Timer {
	private:
		float time;

	public:
		Timer();

		void Update(float);
		void Restart();
		float Get();
	};
#endif