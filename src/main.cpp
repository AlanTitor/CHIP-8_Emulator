#include <iostream>
#include <chrono>

#include "Graphic.h"
#include "ChipEmu.h"

#undef main

const int WindowWidth = 64, WindowHeight = 32;

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

	Graphic platform("CHIP-8 Emulator", WindowWidth * videoScale, WindowHeight * videoScale, WindowWidth, WindowHeight);

	ChipEmu chip8;
	chip8.loadROM(romFilename);

	int videoPitch = sizeof(chip8.video[0]) * WindowWidth;

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