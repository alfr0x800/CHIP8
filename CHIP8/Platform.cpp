#include "Platform.h"

Platform::Platform()
{
	SDL_Init(SDL_INIT_VIDEO);
	_window = SDL_CreateWindow("CHIP8", _width * _scale, _height * _scale, 0);
	_renderer = SDL_CreateRenderer(_window, nullptr);
	SDL_SetRenderVSync(_renderer, SDL_RENDERER_VSYNC_ADAPTIVE);
	SDL_SetRenderScale(_renderer, 10.0f, 10.0f);

	_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _width, _height);
	SDL_SetTextureScaleMode(_texture, SDL_SCALEMODE_NEAREST);
}

Platform::~Platform()
{
	SDL_DestroyTexture(_texture);
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Platform::Clear()
{
	SDL_SetRenderTarget(_renderer, _texture);
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(_renderer);
	SDL_SetRenderTarget(_renderer, nullptr);
	_isModified = true;
}

void Platform::DrawPixel(int x, int y, bool on)
{
	SDL_SetRenderTarget(_renderer, _texture);
	if (on)
		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	else
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderPoint(_renderer, x, y);
	SDL_SetRenderTarget(_renderer, nullptr);
	_isModified = true;
}

/*void Platform::Draw(const std::bitset<_width* _height>& display)
{
	SDL_SetRenderTarget(_renderer, _texture);
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(_renderer);
	for (int y{}; y < _height; y++)
	{
		for (int x{}; x < _width; x++)
		{
			if (display[y * _width + x])
				SDL_SetRenderDrawColor(_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
			else
				SDL_SetRenderDrawColor(_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderPoint(_renderer, x, y);
		}
	}
	SDL_SetRenderTarget(_renderer, nullptr);
	SDL_RenderTexture(_renderer, _texture, nullptr, nullptr);
	SDL_RenderPresent(_renderer);
}*/

void Platform::Update(std::bitset<16>& keypad, const std::bitset<_width* _height>& display)
{
	// Poll for events
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_EVENT_QUIT:
				_isOpen = false;
				break;
			case SDL_EVENT_KEY_DOWN:
			{
				int index = KeyToKeypadIndex(event.key.key);
				if (index > 0)
					keypad[index] = 1;
			}
			break;
			case SDL_EVENT_KEY_UP:
			{
				int index = KeyToKeypadIndex(event.key.key);
				if (index > 0)
					keypad[index] = 0;
			}
			break;
		}
	}

	// Draw the display
	if (_isModified)
	{
		SDL_RenderTexture(_renderer, _texture, nullptr, nullptr);
		SDL_RenderPresent(_renderer);
	}
}

bool Platform::GetIsOpen()
{
	return _isOpen;
}

int Platform::KeyToKeypadIndex(SDL_Keycode key)
{
	switch (key)
	{
		case SDLK_1:
			return 0x01;
		case SDLK_2:
			return 0x02;
		case SDLK_3:
			return 0x03;
		case SDLK_4:
			return 0x0C;
		case SDLK_Q:
			return 0x04;
		case SDLK_W:
			return 0x05;
		case SDLK_E:
			return 0x06;
		case SDLK_R:
			return 0x0D;
		case SDLK_A:
			return 0x07;
		case SDLK_S:
			return 0x08;
		case SDLK_D:
			return 0x09;
		case SDLK_F:
			return 0x0E;
		case SDLK_Z:
			return 0x0A;
		case SDLK_X:
			return 0x00;
		case SDLK_C:
			return 0x0B;
		case SDLK_V:
			return 0x0F;
		default:
			return -1;
	}
}