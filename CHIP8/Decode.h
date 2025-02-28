#pragma once

#include <cstdint>

enum class Opcode
{
	None,
	Cls_00E0,
	Ret_00EE,
	Dw_0nnn,
	Jp_1nnn,
	Call_2nnn,
	Se_3xnn,
	Sne_4xnn,
	Se_5xy0,
	Ld_6xnn,
	Add_7xnn,
	Ld_8xy0,
	Or_8xy1,
	And_8xy2,
	Xor_8xy3,
	Add_8xy4,
	Sub_8xy5,
	Shr_8xy6,
	Subn_8xy7,
	Shl_8xyE,
	Sne_9xy0,
	Ld_Annn,
	Jp_Bnnn,
	Rnd_Cxnn,
	Drw_Dxyn,
	Skp_Ex9E,
	Sknp_ExA1,
	Ld_Fx07,
	Ld_Fx0A,
	Ld_Fx15,
	Ld_Fx18,
	Add_Fx1E,
	Ld_Fx29,
	Ld_Fx33,
	Ld_Fx55,
	Ld_Fx65
};

struct Instruction
{
	Opcode Opcode;
	uint8_t X;
	uint8_t Y;
	uint16_t NNN;
	uint8_t NN;
	uint8_t N;

	Instruction(uint16_t raw);
};