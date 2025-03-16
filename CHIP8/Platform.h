#pragma once

#include <SDL3/SDL.h>

class Platform
{
private:
	static constexpr int _width = 64;
	static constexpr int _height = 32;
	static constexpr int _scale = 10;

	SDL_Window* _window{};
	SDL_Renderer* _renderer{};

	bool _isModified{ true };
	bool _isOpen{ true };
	char _lastKey{};

public:
	Platform();
	~Platform();

	void Clear();
	void DrawPixel(int x, int y, bool state);
	void Update();

	bool GetIsOpen();
	char GetLastKey();
};