#pragma once

#include "Window.h"

#include <array>
#include <string_view>

class CHIP8
{
private:
	static constexpr int _programOffset = 0x200;

	std::array<bool, 64 * 32> _display{};

	std::array<uint8_t, 2048> _memory{};
	std::array<uint16_t, 12> _stack{};

	uint16_t _pc{ _programOffset };
	uint16_t _i{};
	uint8_t _sp{};
	std::array<uint8_t, 16> _v{};

	uint8_t _delayTimer{};
	uint8_t _soundTimer{};

	Window _window;

public:
	CHIP8(const std::string_view& romPath);

	void Run();

private:
	void Execute();
	
	void DisplaySprite(int x, int y, int h);

	char GetKeyConverted();
};