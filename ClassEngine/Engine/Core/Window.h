#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>

#include "Debug.h"

class Window
{
public:
	Window();
	~Window();

	bool OnCreate(std::string name_, int width_, int height_);
	void OnDestroy();
	int GetWidth() const;
	int GetHeight() const;
	SDL_Window* GetWindow() const;

private:
	void SetAttributes();
	int width;
	int height;
	SDL_Window* window;
	SDL_GLContext context;
};

#endif //!WINDOW_H
