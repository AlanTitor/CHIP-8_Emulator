# CHIP-8 SDL Emulator

This is a simple emulator of CHIP-8 System, that capable of loading and playing games in (.ch8) format.
CHIP-8 is an interpreted programming language used in the 1970s to create simple games and programs. This emulator allows you to experience classic CHIP-8 games on your modern computer.
If you want to try it, you need to either download it or build it yourself.

## Building

1. **Download SDL2**
- First you need to download library SDL2 (That code was made on version 2.30.12 and won't work on newer version!).
- [Link](https://github.com/libsdl-org/SDL/releases/tag/release-2.30.12)
- Then put it in the root folder.

2. **Cmake**
- Then you need to download cmake to build the project. Go to official page and download it. [Link](https://cmake.org/download/)
it might request you to put reference to project in to variable PATH.

3. **Build Project**
- Now we can start building. To do that you need to be in root directory and type: <mark>cmake .</mark> (or make folder "build" cd there and type <mark>cmake ..</mark> that's even better).

4. **Compile Project**
- Make new folder Release or Debug cd there and type <mark>cmake --build .</mark> (if you want release version type <mark>cmake --build . --config Release</mark>).

5. **EXE**
- Now you have EXE. To play it you need to do few more steps. First it needs some parameters:
- `<scale>`: The scale factor for the window size (e.g., `10` for a 640x320 window).
- `<delay>`: The delay between cycles in milliseconds (e.g., `1` for fast execution).
- `<rom_file>`: The path to the `.ch8` ROM file you want to play.

6. **Download Games**
- Download some games and enjoy =). Here is [Link to games](https://github.com/dmatlack/chip8)

7. **Controls**:
   - The CHIP-8 keypad is mapped to the following keys on your keyboard:
     ```
     1 2 3 C
     4 5 6 D
     7 8 9 E
     A 0 B F
     ```
   - On a standard QWERTY keyboard, the keys are mapped as follows:
     ```
     1 2 3 4
     Q W E R
     A S D F
     Z X C V
     ```
   - Use these keys to interact with the game.

## File system

- There are a fwe files with code. 

### main.cpp

- First is main file with checking args and main cycle of emulator. It is pretty simple. There is constructor of SDL class "Graphic" that initialized with arguments. Then starts main cycle of game.

### ChipEmu.h and ChipEmu.cpp

- ChipEmu.h just a simple header file with variables and ChipEmu.cpp is realization of it. There are lots of opcodes that you can google and see what it does.

### Graphic.h and Graphic.cpp

- These are SDL class that you can check documentation online and see how it works exactly.

## Conclision
- For more details you can check these guides:

1. https://chip-8.github.io/links/
2. https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
3. https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#8xy4-add
4. https://austinmorlan.com/posts/chip8_emulator/
5. https://en.wikipedia.org/wiki/CHIP-8#Opcode_table