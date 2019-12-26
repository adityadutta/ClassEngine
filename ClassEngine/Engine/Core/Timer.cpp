#include "Timer.h"
#include <iostream>

Timer::Timer(): prevTicks(0), currentTicks(0)
{
}


Timer::~Timer()
{
}

void Timer::Start()
{
	prevTicks = SDL_GetTicks();
	currentTicks = SDL_GetTicks();

	prevTicksHighRes = currentTicksHighRes = high_resolution_clock::now();
}

void Timer::UpdateFrameTicks()
{
	prevTicks = currentTicks;
	currentTicks = SDL_GetTicks();

	//high res
	prevTicksHighRes = currentTicksHighRes;
	currentTicksHighRes = high_resolution_clock::now();
}

float Timer::GetDeltaTime() const
{
	//return static_cast<float>(currentTicks - prevTicks) / 1000.0f;
	//high res
	return duration_cast<duration<float>>(currentTicksHighRes - prevTicksHighRes).count();
}

unsigned int Timer::GetSleepTime(const unsigned fps_) const
{
	//gets milliseconds per frame
	unsigned int milliPerFrame = 1000 / fps_;

	if(milliPerFrame == 0)
	{
		return 0;
	}

	unsigned int sleepTime = milliPerFrame - SDL_GetTicks();
	if (sleepTime > milliPerFrame)
	{
		return milliPerFrame;
	}
}

float Timer::GetCurrentTicks() const
{
	return static_cast<float>(currentTicks) / 1000.0f;
}
