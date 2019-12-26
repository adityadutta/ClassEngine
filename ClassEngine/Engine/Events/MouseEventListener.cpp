#include "MouseEventListener.h"
#include "../Core/CoreEngine.h"

CoreEngine* MouseEventListener::engineInstance = nullptr;
int MouseEventListener::mouseX = 0;
int MouseEventListener::mouseY = 0;
int MouseEventListener::prevMouseX = 0;
int MouseEventListener::prevMouseY = 0;
bool MouseEventListener::firstUpdate = true;

MouseEventListener::~MouseEventListener()
{
	engineInstance = nullptr;
}

void MouseEventListener::RegisterEngineObject(CoreEngine* engine_)
{
	engineInstance = engine_;
}

void MouseEventListener::Update(SDL_Event e_)
{
	switch (e_.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		UpdateMousePosition();
		NotifyOfMousePressed(e_.button.button);
		break;
	case SDL_MOUSEBUTTONUP:
		UpdateMousePosition();
		NotifyOfMouseRelease(e_.button.button);
		break;
	case SDL_MOUSEMOTION:
		UpdateMousePosition();
		NotifyOfMouseMove();
		break;
	case SDL_MOUSEWHEEL:
		NotifyOfMouseScroll(e_.wheel.y);
		break;
	default:
		break;
	}
}

void MouseEventListener::NotifyOfMousePressed(int buttonType_)
{
	if(engineInstance)
	{
		engineInstance->NotifyOfMousePressed(mouseX, mouseY);
	}
}

void MouseEventListener::NotifyOfMouseRelease(int buttonType_)
{
	if(engineInstance)
	{
		engineInstance->NotifyOfMouseRelease(mouseX, mouseY, buttonType_);
	}
}

void MouseEventListener::NotifyOfMouseMove()
{
	if(engineInstance)
	{
		engineInstance->NotifyOfMouseMove(mouseX, mouseY);
	}
}

void MouseEventListener::NotifyOfMouseScroll(int y_)
{
	if(engineInstance)
	{
		engineInstance->NotifyOfMouseScroll(y_);
	}
}

glm::vec2 MouseEventListener::GetPreviousMousePosition()
{
	return glm::vec2(prevMouseX, prevMouseY);
}

glm::vec2 MouseEventListener::GetMousePosition()
{
	return glm::vec2(mouseX, mouseY);
}

glm::vec2 MouseEventListener::GetMouseOffset()
{
	return glm::vec2(mouseX - prevMouseX, prevMouseY - mouseY);
}

void MouseEventListener::UpdateMousePosition()
{
	int tempX, tempY;
	SDL_GetMouseState(&tempX, &tempY);
	tempY = engineInstance->GetScreenSize().y - tempY;

	if (firstUpdate)
	{
		prevMouseX = mouseX = tempX;
		prevMouseY = mouseY = tempY;
		firstUpdate = false;
	}
	else if (mouseX != tempX || mouseY != tempY)
	{
		prevMouseX = mouseX;
		prevMouseY = mouseY;
		mouseX = tempX;
		mouseY = tempY;
	}
}
