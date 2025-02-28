#include "Interpreter.h"

#include <print>

int main(int argc, const char** argv)
{
	if (argc != 2)
	{
		std::println("usage: {} [rom]", argv[0]);
		std::exit(1);
	}

	Interpreter interpreter(argv[0]);
	interpreter.Run();
}