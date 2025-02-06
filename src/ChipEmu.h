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

    ChipEmu();

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
    
    void Table0();
    void Table8();
    void TableE();
    void TableF();
    void OP_NULL();


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