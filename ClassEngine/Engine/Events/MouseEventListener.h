#ifndef MOUSEEVENTLISTENER_H
#define MOUSEEVENTLISTENER_H

#include <SDL.h>
#include <glm/glm.hpp>

class CoreEngine;

class MouseEventListener
{
public:
	MouseEventListener() = delete;
	MouseEventListener(const MouseEventListener&) = delete; //Copy constructor
	MouseEventListener(MouseEventListener&&) = delete;//move constructor
	MouseEventListener operator=(const MouseEventListener&) = delete;
	MouseEventListener operator=(MouseEventListener&&) = delete;
	~MouseEventListener();

	static void RegisterEngineObject(CoreEngine* engine_);
	static void Update(SDL_Event e_);
	static void NotifyOfMousePressed(int buttonType_);
	static void NotifyOfMouseRelease(int buttonType_);
	static void NotifyOfMouseMove();
	static void NotifyOfMouseScroll(int y_);
	static glm::vec2 GetPreviousMousePosition();
	static glm::vec2 GetMousePosition();
	static glm::vec2 GetMouseOffset();
private:
	static CoreEngine* engineInstance;
	static int mouseX, mouseY, prevMouseX, prevMouseY;
	static bool firstUpdate;

	static void UpdateMousePosition();
};

#endif //!MOUSEEVENTLISTENER_H