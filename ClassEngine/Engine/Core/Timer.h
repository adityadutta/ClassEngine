#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

#include <chrono>

using namespace std::chrono;

class Timer
{
public:
	Timer(const Timer&) = delete; //Copy constructor
	Timer(Timer&&) = delete;//move constructor
	Timer operator=(const Timer&) = delete;
	Timer operator=(Timer&&) = delete;

	Timer();
	~Timer();

	//Start Timer
	void Start();

	//Update current ticks
	void UpdateFrameTicks();

	//Get Frame delta time
	float GetDeltaTime() const;


	unsigned int GetSleepTime(const unsigned int fps_) const;

	//Get Current Ticks in seconds
	float GetCurrentTicks() const;

private:
	unsigned int prevTicks;
	unsigned int currentTicks;

	//high res
	high_resolution_clock::time_point prevTicksHighRes;
	high_resolution_clock::time_point currentTicksHighRes;
};

#endif //!TIMER_H