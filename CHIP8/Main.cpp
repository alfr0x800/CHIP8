#include "CHIP8.h"

#include "Decode.h"

#include <print>

int main(int argc, const char** argv)
{
	if (argc != 2)
	{
		std::println("usage: {} [rom]", argv[0]);
		std::exit(1);
	}

	CHIP8 CHIP8(argv[1]);
	CHIP8.Run();
}