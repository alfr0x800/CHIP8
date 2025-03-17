#include "Decode.h"
#include "Font.h"
#include "Interpreter.h"

#include <filesystem>
#include <fstream>
#include <print>

Interpreter::Interpreter(const std::string_view& romPath)
{
	// Load the ROM into memory
	std::ifstream rom(romPath.data(), std::ifstream::binary);
	size_t romSize{ std::filesystem::file_size(romPath) };
	if (romSize < _memory.size() - _pc)
		rom.read((char*)&_memory[_pc], romSize);

	// Load the font into memory
	std::copy(Font::Data.begin(), Font::Data.end(), _memory.begin());
}


void Interpreter::Run()
{
	while (_platform.GetIsOpen())
	{
		Cycle();
		_platform.Update(_keypad, _display);
	}
}

void Interpreter::Cycle()
{
	// Fetch and decode
	Instruction ins((_memory[_pc++] << 8) | _memory[_pc++]);

	// Execute
	switch (ins.Opcode)
	{
		case Opcode::Cls_00E0:
			_display = {};
			_platform.Clear();
			break;
		case Opcode::Ret_00EE:
			_pc = _stack[--_sp];
			break;
		case Opcode::Jp_1nnn:
			_pc = ins.NNN;
			break;
		case Opcode::Call_2nnn:
			_stack[_sp++] = _pc;
			_pc = ins.NNN;
			break;
		case Opcode::Se_3xnn:
			_pc += _v[ins.X] == ins.NN ? 2 : 0;
			break;
		case Opcode::Sne_4xnn:
			_pc += _v[ins.X] != ins.NN ? 2 : 0;
			break;
		case Opcode::Se_5xy0:
			_pc += _v[ins.X] == _v[ins.Y] ? 2 : 0;
			break;
		case Opcode::Ld_6xnn:
			_v[ins.X] = ins.NN;
			break;
		case Opcode::Add_7xnn:
			_v[ins.X] += ins.NN;
			break;
		case Opcode::Ld_8xy0:
			_v[ins.X] = _v[ins.Y];
			break;
		case Opcode::Or_8xy1:
			_v[ins.X] |= _v[ins.Y]; _v[0xF] = 0;
			break;
		case Opcode::And_8xy2:
			_v[ins.X] &= _v[ins.Y]; _v[0xF] = 0;
			break;
		case Opcode::Xor_8xy3:
			_v[ins.X] ^= _v[ins.Y]; _v[0xF] = 0;
			break;
		case Opcode::Add_8xy4:
			_v[ins.X] += _v[ins.Y];
			_v[0xF] = _v[ins.X] + _v[ins.Y] > UINT8_MAX;
			break;
		case Opcode::Sub_8xy5:
			_v[ins.X] -= _v[ins.Y];
			_v[0xF] = _v[ins.X] > _v[ins.Y];
			break;
		case Opcode::Shr_8xy6:
			_v[0xF] = _v[ins.X] & 1;
			_v[ins.X] >>= 1;
			break;
		case Opcode::Subn_8xy7:
			_v[ins.X] = _v[ins.Y] - _v[ins.X];
			_v[0xF] = _v[ins.X] - _v[ins.Y] < 0;
			break;
		case Opcode::Shl_8xyE:
			_v[0xF] = (_v[ins.X] >> 7) & 1;
			_v[ins.X] <<= 1;
			break;
		case Opcode::Sne_9xy0:
			_pc += _v[ins.X] != _v[ins.Y] ? 2 : 0;
			break;
		case Opcode::Ld_Annn:
			_i = ins.NNN;
			break;
		case Opcode::Jp_Bnnn:
			_pc = ins.NNN + _v[0];
			break;
		case Opcode::Rnd_Cxnn:
			_v[ins.X] = std::rand() % ins.NN;
			break;
		case Opcode::Drw_Dxyn:
			for (int yOff{}; yOff < ins.N; yOff++)
				for (int xOff{}; xOff < 8; xOff++)
					if (_memory[_i + yOff] & (0x80 >> xOff))
						DisplaySetPixel(_v[ins.X] + xOff, _v[ins.Y] + yOff);
			break;
		case Opcode::Skp_Ex9E:
			_pc += _keypad[_v[ins.X]] ? 2 : 0;
			break;
		case Opcode::Sknp_ExA1:
			_pc += !_keypad[_v[ins.X]] ? 2 : 0;
			break;
		case Opcode::Ld_Fx07:
			_v[ins.X] = _delayTimer;
			break;
		case Opcode::Ld_Fx0A:
			_pc -= _keypad[_v[ins.X]] ? 0 : 2;
			break;
		case Opcode::Ld_Fx15:
			_delayTimer = _v[ins.X];
			break;
		case Opcode::Ld_Fx18:
			_soundTimer = _v[ins.X];
			break;
		case Opcode::Add_Fx1E:
			_i += _v[ins.X];
			break;
		case Opcode::Ld_Fx29:
			_i = _v[ins.X] * 5;
			break;
		case Opcode::Ld_Fx33:
			_memory[_i] = _v[ins.X] / 100;
			_memory[_i + 1] = (_v[ins.X] / 10) % 10;
			_memory[_i + 2] = (_v[ins.X] % 100) % 10;
			break;
		case Opcode::Ld_Fx55:
			for (int i{}; i <= ins.X; i++)
				_memory[_i++] = _v[i];
			_i++;
			break;
		case Opcode::Ld_Fx65:
			for (int i{}; i <= ins.X; i++)
				_v[i] = _memory[_i++];
			_i++;
			break;
	}

	// Tick timers
	_delayTimer -= _delayTimer > 0;
	_soundTimer -= _soundTimer > 0;

	/*for (int y{}; y < 32; y++)
	{
		for (int x{}; x < 64; x++)
		{
			if (_display[y * 64 + x])
				std::print("#");
			else
				std::print(" ");

		}

		std::print("\n");
	}*/
}

void Interpreter::DisplaySetPixel(int x, int y)
{
	// Horizontal wrapping
	x %= 64;
	// Vertical wrapping
	y %= 32;
	// Set the pixel and flag
	_v[0xF] = _display[y * 64 + x];
	_display[y * 64 + x] = _display[y * 64 + x] ? false : true;
	_platform.DrawPixel(x, y, _display[y * 64 + x]);
}