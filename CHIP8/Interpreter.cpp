#include "Decode.h"
#include "Interpreter.h"

#include <fstream>
#include <print>

Interpreter::Interpreter(const std::string_view& romPath)
{
	// Initialise the memory
	_memory.fill({});
	_stack.fill({});

	// Initialise the registers
	_pc = _programOffset;
	_i = {};
	_sp = {};
	_v.fill({});

	// Initialise the timers
	_delayTimer = {};
	_soundTimer = {};

	// Load the ROM into memory
	std::ifstream rom(romPath.data());
	if (rom.is_open())
	{
		for (size_t i = _programOffset; i < _memory.size() && !rom.eof(); i++)
			_memory[i] = rom.get();
	}

	// Load the font into memory
}

Interpreter::~Interpreter()
{ }

void Interpreter::Run()
{ }

void Interpreter::Execute()
{
	Instruction ins((_memory[_pc] << 8) | _memory[_pc + 1]);
	switch (ins.Opcode)
	{
	case Opcode::Cls_00E0: break;
	case Opcode::Ret_00EE: break;

	case Opcode::Jp_1nnn: _pc = ins.NNN; break;

	case Opcode::Call_2nnn: break;

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
	
	case Opcode::Rnd_Cxnn: break;
	case Opcode::Drw_Dxyn: break;
	case Opcode::Skp_Ex9E: break;
	case Opcode::Sknp_ExA1: break;
	
	case Opcode::Ld_Fx07: _v[ins.X] = _delayTimer; break;
	
	case Opcode::Ld_Fx0A: break;
	
	case Opcode::Ld_Fx15: _delayTimer = _v[ins.X]; break;
	case Opcode::Ld_Fx18: _soundTimer = _v[ins.X]; break;
	case Opcode::Add_Fx1E: _i += _v[ins.X]; break;
	
	case Opcode::Ld_Fx29: break;
	case Opcode::Ld_Fx33: break;
	case Opcode::Ld_Fx55: break;
	case Opcode::Ld_Fx65: break;
	}
}