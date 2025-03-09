#pragma once

#include <array>

#include <SDL3/SDL.h>

class Window
{
public:
	bool ShowRegisters{ false };
	bool ShowDebugger{ false };
	bool ShowDiassembler{ false };

private:
	SDL_Window* _window{};
	SDL_Renderer* _renderer{};
	SDL_Texture* _displayTx{};

	bool _isOpen{ true };
	char _lastKeyPressed{};

public:
	Window();
	~Window();

	void PollEvents();

	void BeginDraw();
	void EndDraw();
	void DrawTools();
	void DrawDisplay(const std::array<bool, 64 * 32>& display);
	void DrawRegisters(uint16_t pc, uint16_t i, uint16_t sp, const std::array<uint8_t, 16>& v);

	bool GetIsOpen();
	char GetLastKeyPressed();
};