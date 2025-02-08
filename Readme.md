# CHIP-8 SDL Emulator

This is a simple emulator of the CHIP-8 system, capable of loading and playing games in `.ch8` format. CHIP-8 is an interpreted programming language used in the 1970s to create simple games and programs. This emulator allows you to experience classic CHIP-8 games on your modern computer. If you want to try it, you need to either download it or build it yourself.

## Building

1. **Download SDL2**:
   - First, you need to download the SDL2 library (this code was made for version 2.30.12 and won't work on newer versions).
   - [Link](https://github.com/libsdl-org/SDL/releases/tag/release-2.30.12)
   - Then, place it in the root folder.

2. **CMake**:
   - Next, you need to download CMake to build the project. Go to the official page and download it. [Link](https://cmake.org/download/)
   - During installation, you might be asked to add CMake to the system `PATH`.

3. **Build the Project**:
   - Now, you can start building. To do this, navigate to the root directory and type:  
     ```bash
     cmake .
     ```
   - Alternatively, you can create a `build` folder, navigate to it, and type:  
     ```bash
     cmake ..
     ```  
     (This is the preferred method.)

4. **Compile the Project**:
   - Create a new folder named `Release` or `Debug`, navigate to it, and type:  
     ```bash
     cmake --build .
     ```  
   - If you want a Release build, use:  
     ```bash
     cmake --build . --config Release
     ```

5. **Run the Executable**:
   - After compiling, you will have an executable file. To play a game, you need to provide some parameters. For example:  
     ```bash
     ./main 10 1 "name_of_game.ch8"
     ```  
   - Here, `10` stands for the resolution scale of the game, and `1` is the delay between cycles in milliseconds (adjust it as needed for different games).

6. **Copy SDL2.dll**:
   - For the program to work, you need to copy the `SDL2.dll` file (from the SDL2 library you downloaded earlier) into the same folder as the compiled executable (e.g., the `Release` or `Debug` folder).

7. **Download Games**:
   - Download some games and enjoy! Here is a [link to games](https://github.com/dmatlack/chip8).

8. **Controls**:
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

## File Structure

The project consists of the following files:

### `main.cpp`
- This is the main file that checks the command-line arguments and runs the main loop of the emulator. It initializes the SDL class `Graphic` with the provided arguments and starts the emulation cycle.

### `ChipEmu.h` and `ChipEmu.cpp`
- `ChipEmu.h` is a header file containing the class definition for the CHIP-8 emulator.
- `ChipEmu.cpp` implements the CHIP-8 emulator, including all opcodes and their functionality.

### `Graphic.h` and `Graphic.cpp`
- These files contain the SDL class for handling graphics. You can check the SDL documentation online to understand how it works in detail.

## Conclusion

For more details, you can check these guides:
1. [CHIP-8 Links](https://chip-8.github.io/links/)
2. [How to Write an Emulator (CHIP-8 Interpreter)](https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)
3. [Write a CHIP-8 Emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#8xy4-add)
4. [CHIP-8 Emulator Guide by Austin Morlan](https://austinmorlan.com/posts/chip8_emulator/)
5. [CHIP-8 Opcode Table (Wikipedia)](https://en.wikipedia.org/wiki/CHIP-8#Opcode_table)