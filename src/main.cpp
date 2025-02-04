#include <iostream>
#include <chrono>

#include "Platform.h"
#include "ChipEmu.h"

#undef main

const int WIDTH = 64, HEIGHT = 32;

int main(int argc, char** argv)
{
    if (argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
		std::exit(EXIT_FAILURE);
	}

	int videoScale = std::stoi(argv[1]);
	int cycleDelay = std::stoi(argv[2]);
	char const* romFilename = argv[3];

	Platform platform("CHIP-8 Emulator", WIDTH * videoScale, HEIGHT * videoScale, WIDTH, HEIGHT);

	ChipEmu chip8;
	chip8.loadROM(romFilename);

	int videoPitch = sizeof(chip8.video[0]) * WIDTH;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;

	while (!quit)
	{
		quit = platform.ProcessInput(chip8.keyPad);

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		if (dt > cycleDelay)
		{
			lastCycleTime = currentTime;

			chip8.cycle();

			platform.Update(chip8.video, videoPitch);
		}
	}
    return 0;
}