#include "Platform.h"

Platform::Platform()
{
	SDL_Init(SDL_INIT_VIDEO);
	_window = SDL_CreateWindow("CHIP8", _width * _scale, _height * _scale, 0);
	_renderer = SDL_CreateRenderer(_window, nullptr);
	SDL_SetRenderVSync(_renderer, SDL_RENDERER_VSYNC_ADAPTIVE);
	SDL_SetRenderScale(_renderer, _scale, _scale);
	SDL_StartTextInput(_window);
}

Platform::~Platform()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Platform::Clear()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(_renderer);
	_isModified = true;
}

void Platform::DrawPixel(int x, int y, bool state)
{
	if (state)
		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	else
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderPoint(_renderer, (float)x, (float)y);
	SDL_RenderPresent(_renderer);
	_isModified = true;
}

void Platform::Update()
{
	// Handle events
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			_isOpen = false;
			break;
		case SDL_EVENT_TEXT_INPUT:
			_lastKey = *event.text.text;
			break;
		}
	}

	// Show changes to the window if needed
	if (_isModified)
	{
		SDL_RenderPresent(_renderer);
		_isModified = false;
	}
}

bool Platform::GetIsOpen()
{
	return _isOpen;
}

char Platform::GetLastKey()
{
	return _lastKey;
}