#pragma once

#include <array>
#include <string_view>

class Interpreter
{
private:
	static constexpr int _programOffset = 0x512;

	std::array<uint8_t, 2048> _memory;
	std::array<uint16_t, 12> _stack;

	std::uint16_t _pc;
	std::uint16_t _i;
	std::uint8_t _sp;
	std::array<uint8_t, 16> _v;

	std::uint8_t _delayTimer;
	std::uint8_t _soundTimer;

public:
	Interpreter(const std::string_view& romPath);
	~Interpreter();

	void Run();

private:
	void Execute();
};