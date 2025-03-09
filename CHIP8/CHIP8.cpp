#include "Decode.h"
#include "Font.h"
#include "CHIP8.h"

#include <filesystem>
#include <fstream>
#include <print>

#include <imgui.h>

CHIP8::CHIP8(const std::string_view& romPath) 
{
	std::ifstream rom(romPath.data());
	size_t romSize { std::filesystem::file_size(romPath) };
	if (romSize < _memory.size() - _pc)
		rom.read((char*)&_memory[_pc], romSize);

	std::copy(Font::Data.begin(), Font::Data.end(), _memory.begin());
}

void CHIP8::Run()
{
	while (_window.GetIsOpen())
	{
		_window.PollEvents();
		
		Execute();
		
		_window.BeginDraw();
		_window.DrawTools();
		_window.DrawDisplay(_display);
		if (_window.ShowRegisters)
			_window.DrawRegisters(_pc, _i, _sp, _v);
		_window.EndDraw();
	}
}

void CHIP8::Execute()
{
	Instruction ins((_memory[_pc++] << 8) | _memory[_pc++]);
	switch (ins.Opcode)
	{
		case Opcode::Cls_00E0: DisplayClear(); break;
		case Opcode::Ret_00EE: _pc = _stack[_sp--]; break;
		case Opcode::Jp_1nnn: _pc = ins.NNN; break;
		case Opcode::Call_2nnn: _stack[_sp++] = _pc; _pc = ins.NNN; break;
		case Opcode::Se_3xnn: _pc += _v[ins.X] == ins.NN; break;
		case Opcode::Sne_4xnn: _pc += _v[ins.X] != ins.NN; break;
		case Opcode::Se_5xy0: _pc += _v[ins.X] == _v[ins.Y]; break;
		case Opcode::Ld_6xnn: _v[ins.X] = ins.NN; break;
		case Opcode::Add_7xnn: _v[ins.X] += ins.NN; break;
		case Opcode::Ld_8xy0: _v[ins.X] = _v[ins.Y]; break;
		case Opcode::Or_8xy1: _v[ins.X] |= _v[ins.Y]; break;
		case Opcode::And_8xy2: _v[ins.X] &= _v[ins.Y]; break;
		case Opcode::Xor_8xy3: _v[ins.X] ^= _v[ins.Y]; break;
		case Opcode::Add_8xy4: _v[0xF] = _v[ins.X] + _v[ins.Y] > UINT8_MAX; _v[ins.X] += _v[ins.Y]; break;
		case Opcode::Sub_8xy5: _v[0xF] = _v[ins.X] - _v[ins.Y] < 0; _v[ins.X] -= _v[ins.Y]; break;
		case Opcode::Shr_8xy6: _v[ins.X] = _v[ins.Y]; _v[0xF] = _v[ins.X] & 1; _v[ins.X] >>= 1; break;
		case Opcode::Subn_8xy7: _v[0xF] = _v[ins.X] - _v[ins.Y] < 0; _v[ins.X] = _v[ins.Y] - _v[ins.X]; break;
		case Opcode::Shl_8xyE: _v[ins.X] = _v[ins.Y]; _v[0xF] = (_v[ins.X] >> 7) & 1; _v[ins.X] <<= 1; break;
		case Opcode::Sne_9xy0: _pc += _v[ins.X] != _v[ins.Y]; break;
		case Opcode::Ld_Annn: _i = ins.NNN; break;
		case Opcode::Jp_Bnnn: _pc = ins.NNN + _v[0]; break;
		case Opcode::Rnd_Cxnn: _v[ins.X] = std::rand() % ins.NN; break;
		case Opcode::Drw_Dxyn: DisplaySprite(ins.X, ins.Y, ins.N); break;
		case Opcode::Skp_Ex9E: _pc += GetKeyConverted() == _v[ins.X] ? 2 : 0; break;
		case Opcode::Sknp_ExA1: _pc += GetKeyConverted() != _v[ins.X] ? 2 : 0; break;
		case Opcode::Ld_Fx07: _v[ins.X] = _delayTimer; break;
		case Opcode::Ld_Fx0A: _pc -= GetKeyConverted() == -1 ? 2 : 0; _v[ins.X] = GetKeyConverted(); break;
		case Opcode::Ld_Fx15: _delayTimer = _v[ins.X]; break;
		case Opcode::Ld_Fx18: _soundTimer = _v[ins.X]; break;
		case Opcode::Add_Fx1E: _i += _v[ins.X]; break;
		case Opcode::Ld_Fx29: _i = _v[ins.X] * 5; break;

		case Opcode::Ld_Fx33: break;

		case Opcode::Ld_Fx55: std::copy(_v.begin(), _v.begin() + ins.X, _memory.begin() + _i); break;
		case Opcode::Ld_Fx65: std::copy(_memory.begin() + _i, _memory.begin() + _i + ins.X, _v.begin()); break;
	}
}

void CHIP8::DisplayClear()
{
	_display = {};
}

void CHIP8::DisplaySprite(int x, int y, int h)
{
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			uint8_t spritePixel = _memory[_i + y] & (0x80 >> x);

			if (spritePixel)
			{
				if (_display[(y * 64) + x])
					_v[0xF] = 1;

				_display[(y * 64) + x] ^= 1;
			}
		}
	}
}

char CHIP8::GetKeyConverted()
{
	switch (std::tolower(_window.GetLastKeyPressed()))
	{
		case '1': return 0x01;
		case '2': return 0x02;
		case '3': return 0x03;
		case '4': return 0x0C;
		case 'q': return 0x04;
		case 'w': return 0x05;
		case 'e': return 0x06;
		case 'r': return 0x0D;
		case 'a': return 0x07;
		case 's': return 0x08;
		case 'd': return 0x09;
		case 'f': return 0x0E;
		case 'z': return 0x0A;
		case 'x': return 0x00;
		case 'c': return 0x0B;
		case 'v': return 0x0F;
		default: return -1;
	}
}
