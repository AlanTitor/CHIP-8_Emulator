/*
0x000-0x1FF: Originally reserved for the CHIP-8 interpreter, but in our modern emulator we will just never write to or read from that area. Except for…
0x050-0x0A0: Storage space for the 16 built-in characters (0 through F), which we will need to manually put into our memory because ROMs will be looking for those characters.
0x200-0xFFF: Instructions from the ROM will be stored starting at 0x200, and anything left after the ROM’s space is free to use.
*/


#pragma once

#include<random>

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;

class ChipEmu{
public:

    ChipEmu() : randGen(std::chrono::system_clock::now().time_since_epoch().count()), randByte(0, 255U){
        table[0x0] = &ChipEmu::Table0;
		table[0x1] = &ChipEmu::OP_1nnn;
		table[0x2] = &ChipEmu::OP_2nnn;
		table[0x3] = &ChipEmu::OP_3xkk;
		table[0x4] = &ChipEmu::OP_4xkk;
		table[0x5] = &ChipEmu::OP_5xy0;
		table[0x6] = &ChipEmu::OP_6xkk;
		table[0x7] = &ChipEmu::OP_7xkk;
		table[0x8] = &ChipEmu::Table8;
		table[0x9] = &ChipEmu::OP_9xy0;
		table[0xA] = &ChipEmu::OP_Annn;
		table[0xB] = &ChipEmu::OP_Bnnn;
		table[0xC] = &ChipEmu::OP_Cxkk;
		table[0xD] = &ChipEmu::OP_Dxyn;
		table[0xE] = &ChipEmu::TableE;
		table[0xF] = &ChipEmu::TableF;

		for (size_t i = 0; i <= 0xE; i++)
		{
			table0[i] = &ChipEmu::OP_NULL;
			table8[i] = &ChipEmu::OP_NULL;
			tableE[i] = &ChipEmu::OP_NULL;
		}

		table0[0x0] = &ChipEmu::OP_00E0;
		table0[0xE] = &ChipEmu::OP_00EE;

		table8[0x0] = &ChipEmu::OP_8xy0;
		table8[0x1] = &ChipEmu::OP_8xy1;
		table8[0x2] = &ChipEmu::OP_8xy2;
		table8[0x3] = &ChipEmu::OP_8xy3;
		table8[0x4] = &ChipEmu::OP_8xy4;
		table8[0x5] = &ChipEmu::OP_8xy5;
		table8[0x6] = &ChipEmu::OP_8xy6;
		table8[0x7] = &ChipEmu::OP_8xy7;
		table8[0xE] = &ChipEmu::OP_8xyE;

		tableE[0x1] = &ChipEmu::OP_ExA1;
		tableE[0xE] = &ChipEmu::OP_Ex9E;

		for (size_t i = 0; i <= 0x65; i++)
		{
			tableF[i] = &ChipEmu::OP_NULL;
		}

		tableF[0x07] = &ChipEmu::OP_Fx07;
		tableF[0x0A] = &ChipEmu::OP_Fx0A;
		tableF[0x15] = &ChipEmu::OP_Fx15;
		tableF[0x18] = &ChipEmu::OP_Fx18;
		tableF[0x1E] = &ChipEmu::OP_Fx1E;
		tableF[0x29] = &ChipEmu::OP_Fx29;
		tableF[0x33] = &ChipEmu::OP_Fx33;
		tableF[0x55] = &ChipEmu::OP_Fx55;
		tableF[0x65] = &ChipEmu::OP_Fx65;


        PC = START_ADDRESS;
    
        for(unsigned int i = 0; i < 80; ++i){
            memory[FONTSET_START_ADDRESS + i] = fontSet[i];
        }
    }


    unsigned char regV[16]{};

    unsigned char memory[4096]{};

    unsigned short I = 0;

    unsigned short PC = 0;

    unsigned short stack[16]{};

    unsigned char SP = 0;

    unsigned char keyPad[16]{};

    unsigned long video[64*32]{};
    
    unsigned char delayTimer = 0;

    unsigned char soundTimer = 0;

    unsigned short opcode;

    unsigned char fontSet[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    std::default_random_engine randGen;
    std::uniform_int_distribution<unsigned int> randByte;

    void loadROM(char const* filename);
    void cycle();

    typedef void (ChipEmu::*Chip8Func)();
	Chip8Func table[0xF + 1];
	Chip8Func table0[0xE + 1];
	Chip8Func table8[0xE + 1];
	Chip8Func tableE[0xE + 1];
	Chip8Func tableF[0x65 + 1];


    void Table0()
	{
		((*this).*(table0[opcode & 0x000Fu]))();
	}

	void Table8()
	{
		((*this).*(table8[opcode & 0x000Fu]))();
	}

	void TableE()
	{
		((*this).*(tableE[opcode & 0x000Fu]))();
	}

	void TableF()
	{
		((*this).*(tableF[opcode & 0x00FFu]))();
	}

	void OP_NULL()
	{}


    //CLS
    void OP_00E0();
    //RET
    void OP_00EE();
    //JP addr
    void OP_1nnn();
    //CALL addr
    void OP_2nnn();
    //SE Vx, byte
    void OP_3xkk();
    //SNE Vx, byte
    void OP_4xkk();
    //SE Vx, Vy
    void OP_5xy0();
    //LD Vx, byte
    void OP_6xkk();
    //ADD Vx, byte
    void OP_7xkk();
    //ADD Vx, Vy
    void OP_8xy0();
    //OR Vx, Vy
    void OP_8xy1();
    //ADN Vx, Vy
    void OP_8xy2();
    //XOR Vx, Vy
    void OP_8xy3();
    //ADD Vx, Vy
    void OP_8xy4();
    //SUB Vx, Vy
    void OP_8xy5();
    //SHR Vx
    void OP_8xy6();
    //SUBN Vx, Vy
    void OP_8xy7();
    //SHL Vx, {, Vy}
    void OP_8xyE();
    //SNE Vx, Vy
    void OP_9xy0();
    //LD I, addr
    void OP_Annn();
    //JP V0, addr
    void OP_Bnnn();
    //RND Vx, byte
    void OP_Cxkk();
    //DRW Vx, Vy, nibble
    void OP_Dxyn();
    //SKP Vx
    void OP_Ex9E();
    //SKNP Vx
    void OP_ExA1();
    //LD Vx, Dt
    void OP_Fx07();
    //LD Vx, K
    void OP_Fx0A();
    //LD DT, Vx
    void OP_Fx15();
    //LD ST, Vx
    void OP_Fx18();
    //ADD I, Vx
    void OP_Fx1E();
    //LD F, Vx
    void OP_Fx29();
    //LD B, Vx
    void OP_Fx33();
    //LD [I], Vx
    void OP_Fx55();
    //LD Vx, [I]
    void OP_Fx65();
};