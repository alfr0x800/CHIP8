#pragma once

#include <bitset>

#include <SDL3/SDL.h>

class Platform
{
private:
	static constexpr int _width = 64;
	static constexpr int _height = 32;
	static constexpr int _scale = 10;

	SDL_Window* _window{};
	SDL_Renderer* _renderer{};
	SDL_Texture* _texture{};

	bool _isModified{ true };
	bool _isOpen{ true };
	SDL_Keycode _lastKey{};

public:
	Platform();
	~Platform();

	void Clear();
	void DrawPixel(int x, int y, bool on);
	void Update(std::bitset<16>& keypad, const std::bitset<_width* _height>& display);

	bool GetIsOpen();

private:
	int KeyToKeypadIndex(SDL_Keycode key);
};