#include "Interpreter.h"
#include "Platform.h"

#include <print>

int main(int argc, const char** argv)
{
	if (argc != 2)
	{
		std::println("usage: {} [rom]", argv[0]);
		std::exit(1);
	}

	Platform platform;
	Interpreter interpterer(&platform, argv[1]);

	while (platform.GetIsOpen())
	{
		interpterer.Cycle();
		platform.Update();
	}
}