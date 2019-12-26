#include "EventListener.h"
#include "../Core/CoreEngine.h"

EventListener::~EventListener()
{
}

void EventListener::Update()
{
	SDL_Event SDLEvent;
	while (SDL_PollEvent(&SDLEvent))
	{
		if (SDLEvent.type == SDL_QUIT)
		{
			CoreEngine::GetInstance()->ExitGame();
		}
		switch (SDLEvent.type)
		{
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
			MouseEventListener::Update(SDLEvent);
			break;
		default:
			break;
		}
	}
}
