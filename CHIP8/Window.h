#pragma once

#include <SDL3/SDL.h>

class Window
{
private:
	SDL_Window* _window {};
	SDL_Renderer* _renderer {};

	bool _isOpen { true };
	char _lastKeyPressed {};

public:
	Window();
	~Window();

	void PollEvents();
	void Redraw();

	bool GetIsOpen();
	char GetLastKeyPressed();

private:
};