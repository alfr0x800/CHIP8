#include "Decode.h"

Instruction::Instruction(uint16_t raw)
{
	X = raw & 0x0F00;
	Y = raw & 0x00F0;
	NNN = raw & 0x0FFF;
	NN = raw & 0x00FF;
	N = raw & 0x000F;

	switch (raw & 0xF000)
	{
	case 0x0000:
		if (NN == 0xE0) Opcode = Opcode::Cls_00E0;
		else if (NN == 0xEE) Opcode = Opcode::Ret_00EE;
		else Opcode = Opcode::None;
		break;

	case 0x1000: Opcode = Opcode::Jp_1nnn; break;
	case 0x2000: Opcode = Opcode::Call_2nnn; break;
	case 0x3000: Opcode = Opcode::Se_3xnn; break;
	case 0x4000: Opcode = Opcode::Sne_4xnn; break;
	case 0x5000: Opcode = Opcode::Se_5xy0; break;
	case 0x6000: Opcode = Opcode::Ld_6xnn; break;
	case 0x7000: Opcode = Opcode::Add_7xnn; break;

	case 0x8000:
		switch (N)
		{
		case 0x0: Opcode = Opcode::Ld_8xy0; break;
		case 0x1: Opcode = Opcode::Or_8xy1; break;
		case 0x2: Opcode = Opcode::And_8xy2; break;
		case 0x3: Opcode = Opcode::Xor_8xy3; break;
		case 0x4: Opcode = Opcode::Add_8xy4; break;
		case 0x5: Opcode = Opcode::Sub_8xy5; break;
		case 0x6: Opcode = Opcode::Shr_8xy6; break;
		case 0x7: Opcode = Opcode::Subn_8xy7; break;
		case 0xE: Opcode = Opcode::Shl_8xyE; break;
		default: Opcode = Opcode::None; break;
		}
		break;
	
	case 0x9000: Opcode = Opcode::Sne_9xy0; break;
	case 0xA000: Opcode = Opcode::Ld_Annn; break;
	case 0xB000: Opcode = Opcode::Jp_Bnnn; break;
	case 0xC000: Opcode = Opcode::Rnd_Cxnn; break;
	case 0xD000: Opcode = Opcode::Drw_Dxyn; break;
	
	case 0xE000:
		if (NN == 0x9E) Opcode = Opcode::Skp_Ex9E;
		else if (NN == 0xA1) Opcode = Opcode::Sknp_ExA1;
		else Opcode = Opcode::None;
		break;

	case 0xF000:
		switch (NN)
		{
		case 0x07: Opcode = Opcode::Ld_Fx07; break;
		case 0x0A: Opcode = Opcode::Ld_Fx0A; break;
		case 0x15: Opcode = Opcode::Ld_Fx15; break;
		case 0x18: Opcode = Opcode::Ld_Fx18; break;
		case 0x1E: Opcode = Opcode::Add_Fx1E; break;
		case 0x29: Opcode = Opcode::Ld_Fx29; break;
		case 0x33: Opcode = Opcode::Ld_Fx33; break;
		case 0x55: Opcode = Opcode::Ld_Fx55; break;
		case 0x65: Opcode = Opcode::Ld_Fx65; break;
		default: Opcode = Opcode::None; break;
		}
		break;

	default: Opcode = Opcode::None; break;
	}
}