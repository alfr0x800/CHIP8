#pragma once

#include "Platform.h"

#include <array>
#include <bitset>
#include <string_view>

class Interpreter
{
private:
	Platform* _platform{};

	std::bitset<64 * 32> _display{};

	std::array<uint8_t, 2048> _memory{};
	std::array<uint16_t, 12> _stack{};

	uint16_t _pc{ 0x200 };
	uint16_t _i{};
	uint8_t _sp{};
	std::array<uint8_t, 16> _v{};

	uint8_t _delayTimer{};
	uint8_t _soundTimer{};

public:
	Interpreter(Platform* platform, const std::string_view& romPath);

	void Cycle();

private:
	void DisplaySetPixel(int x, int y);
	void DisplaySprite(int x, int y, int h);

	char GetKeyConverted();
};