#include "Decode.h"
#include "Font.h"
#include "Interpreter.h"

#include <fstream>

Interpreter::Interpreter(const std::string_view& romPath)
{
	std::ifstream rom(romPath.data());
	if (rom.is_open())
		for (size_t i = _pc; i < _memory.size() && !rom.eof(); i++)
			_memory[i] = rom.get();

	std::copy(Font::Data.begin(), Font::Data.end(), _memory.begin());
}

Interpreter::~Interpreter()
{ }

void Interpreter::Run()
{
	while (_window.GetIsOpen())
	{
		_window.PollEvents();
		_window.Redraw();
	}
}

void Interpreter::Execute()
{
	Instruction ins((_memory[_pc] << 8) | _memory[_pc + 1]);
	switch (ins.Opcode)
	{
	case Opcode::Cls_00E0: break;

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

	case Opcode::Drw_Dxyn: break;

	case Opcode::Skp_Ex9E: _pc += GetKeyConverted() == _v[ins.X] ? 2 : 0; break;
	case Opcode::Sknp_ExA1: _pc += GetKeyConverted() != _v[ins.X] ? 2 : 0; break;
	case Opcode::Ld_Fx07: _v[ins.X] = _delayTimer; break;
	case Opcode::Ld_Fx0A: _pc -= GetKeyConverted() == -1 ? 2 : 0; _v[ins.X] = GetKeyConverted(); break;
	case Opcode::Ld_Fx15: _delayTimer = _v[ins.X]; break;
	case Opcode::Ld_Fx18: _soundTimer = _v[ins.X]; break;
	case Opcode::Add_Fx1E: _i += _v[ins.X]; break;
	
	case Opcode::Ld_Fx29: break;
	case Opcode::Ld_Fx33: break;

	case Opcode::Ld_Fx55: std::copy(_v.begin(), _v.begin() + ins.X, _memory.begin() + _i); break;
	case Opcode::Ld_Fx65: std::copy(_memory.begin() + _i, _memory.begin() + _i + ins.X, _v.begin()); break;
	}
}

char Interpreter::GetKeyConverted()
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