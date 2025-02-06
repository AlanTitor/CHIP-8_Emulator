#include <fstream>
#include <chrono>
#include <cstring>

#include "ChipEmu.h"

typedef void (ChipEmu::*Chip8Func)();

const unsigned int VIDEO_WIDTH = 64;
const unsigned int VIDEO_HEIGHT = 32;

Chip8Func table[0xF + 1];
Chip8Func table0[0xE + 1];
Chip8Func table8[0xE + 1];
Chip8Func tableE[0xE + 1];
Chip8Func tableF[0x65 + 1];

ChipEmu::ChipEmu() : randGen(std::chrono::system_clock::now().time_since_epoch().count()), randByte(0, 255U)
{

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

    for (unsigned int i = 0; i < 80; ++i)
    {
        memory[FONTSET_START_ADDRESS + i] = fontSet[i];
    }
}

void ChipEmu::cycle()
{
    opcode = (memory[PC] << 8U) | memory[PC + 1];

    PC += 2;

    ((*this).*(table[(opcode & 0xF000U) >> 12U]))();

    if (delayTimer > 0)
    {
        --delayTimer;
    }
    if (soundTimer > 0)
    {
        --soundTimer;
    }
}

void ChipEmu::loadROM(char const *filename)
{

    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open())
    {
        std::streampos size = file.tellg();
        char *buffer = new char[size];

        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        for (long i = 0; i < size; ++i)
        {
            memory[START_ADDRESS + i] = buffer[i];
        }

        delete[] buffer;
    }
}

//Pointer tables
void ChipEmu::Table0()
{
    ((*this).*(table0[opcode & 0x000Fu]))();
}

void ChipEmu::Table8()
{
    ((*this).*(table8[opcode & 0x000Fu]))();
}

void ChipEmu::TableE()
{
    ((*this).*(tableE[opcode & 0x000Fu]))();
}

void ChipEmu::TableF()
{
    ((*this).*(tableF[opcode & 0x00FFu]))();
}

void ChipEmu::OP_NULL(){}



//Opcodes block
void ChipEmu::OP_00E0()
{
    memset(video, 0, sizeof(video));
}

void ChipEmu::OP_00EE()
{
    --SP;
    PC = stack[SP];
}

void ChipEmu::OP_1nnn()
{
    unsigned short address = opcode & 0x0FFFU;
    PC = address;
}

void ChipEmu::OP_2nnn()
{
    unsigned short address = opcode & 0x0FFFU;
    stack[SP] = PC;
    ++SP;
    PC = address;
}

void ChipEmu::OP_3xkk()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short byte = opcode & 0x00FFU;

    if (regV[Vx] == byte)
    {
        PC += 2;
    }
}

void ChipEmu::OP_4xkk()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short byte = opcode & 0x00FFU;

    if (regV[Vx] != byte)
    {
        PC += 2;
    }
}

void ChipEmu::OP_5xy0()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    if (regV[Vx] == regV[Vy])
    {
        PC += 2;
    }
}

void ChipEmu::OP_6xkk()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short byte = opcode & 0x00FFU;

    regV[Vx] = byte;
}

void ChipEmu::OP_7xkk()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short byte = opcode & 0x00FFU;

    regV[Vx] += byte;
}

void ChipEmu::OP_8xy0()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    regV[Vx] = regV[Vy];
}

void ChipEmu::OP_8xy1()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    regV[Vx] |= regV[Vy];
}

void ChipEmu::OP_8xy2()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    regV[Vx] &= regV[Vy];
}

void ChipEmu::OP_8xy3()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    regV[Vx] ^= regV[Vy];
}

void ChipEmu::OP_8xy4()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    unsigned short sum = regV[Vx] + regV[Vy];

    if (sum > 255U)
    {
        regV[0xF] = 1;
    }
    else
    {
        regV[0xF] = 0;
    }

    regV[Vx] = sum & 0xFFU;
}

void ChipEmu::OP_8xy5()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    if (regV[Vx] > regV[Vy])
    {
        regV[0xF] = 1;
    }
    else
    {
        regV[0xF] = 0;
    }

    regV[Vx] -= regV[Vy];
}

void ChipEmu::OP_8xy6()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;

    regV[0xF] = (regV[Vx] & 0x1U);

    regV[Vx] >>= 1;
}

void ChipEmu::OP_8xy7()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    if (regV[Vy] > regV[Vx])
    {
        regV[0xF] = 1;
    }
    else
    {
        regV[0xF] = 0;
    }

    regV[Vx] = regV[Vy] - regV[Vx];
}

void ChipEmu::OP_8xyE()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;

    // Save MSB in VF
    regV[0xF] = (regV[Vx] & 0x80u) >> 7u;

    regV[Vx] <<= 1;
}

void ChipEmu::OP_9xy0()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    if (regV[Vx] != regV[Vy])
    {
        PC += 2;
    }
}

void ChipEmu::OP_Annn()
{
    I = opcode & 0x0FFFU;
}

void ChipEmu::OP_Bnnn()
{
    PC = regV[0] & 0x0FFFU;
}

void ChipEmu::OP_Cxkk()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short byte = opcode & 0x00FFU;

    regV[Vx] = randByte(randGen) & byte;
}

void ChipEmu::OP_Dxyn()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;
    unsigned short height = opcode & 0x000FU;

    unsigned short xPos = regV[Vx] % VIDEO_WIDTH;
    unsigned short yPos = regV[Vy] % VIDEO_HEIGHT;

    regV[0xF] = 0;

    for (unsigned int row = 0; row < height; ++row)
    {
        unsigned short spriteByte = memory[I + row];

        for (unsigned int col = 0; col < 8; ++col)
        {
            unsigned short spritePixel = spriteByte & (0x80U >> col);
            unsigned long *screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

            if (spritePixel)
            {
                if (*screenPixel == 0xFFFFFFFF)
                {
                    regV[0xF] = 1;
                }
                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void ChipEmu::OP_Ex9E()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short key = regV[Vx];

    if (keyPad[key])
    {
        PC += 2;
    }
}

void ChipEmu::OP_ExA1()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short key = regV[Vx];

    if (!keyPad[key])
    {
        PC += 2;
    }
}

void ChipEmu::OP_Fx07()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;

    regV[Vx] = delayTimer;
}

void ChipEmu::OP_Fx0A()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;

    if (keyPad[0])
    {
        regV[Vx] = 0;
    }
    else if (keyPad[1])
    {
        regV[Vx] = 1;
    }
    else if (keyPad[2])
    {
        regV[Vx] = 2;
    }
    else if (keyPad[3])
    {
        regV[Vx] = 3;
    }
    else if (keyPad[4])
    {
        regV[Vx] = 4;
    }
    else if (keyPad[5])
    {
        regV[Vx] = 5;
    }
    else if (keyPad[6])
    {
        regV[Vx] = 6;
    }
    else if (keyPad[7])
    {
        regV[Vx] = 7;
    }
    else if (keyPad[8])
    {
        regV[Vx] = 8;
    }
    else if (keyPad[9])
    {
        regV[Vx] = 9;
    }
    else if (keyPad[10])
    {
        regV[Vx] = 10;
    }
    else if (keyPad[11])
    {
        regV[Vx] = 11;
    }
    else if (keyPad[12])
    {
        regV[Vx] = 12;
    }
    else if (keyPad[13])
    {
        regV[Vx] = 13;
    }
    else if (keyPad[14])
    {
        regV[Vx] = 14;
    }
    else if (keyPad[15])
    {
        regV[Vx] = 15;
    }
    else
    {
        PC -= 2;
    }
}

void ChipEmu::OP_Fx15()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    delayTimer = regV[Vx];
}

void ChipEmu::OP_Fx18()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    soundTimer = regV[Vx];
}

void ChipEmu::OP_Fx1E()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    I += regV[Vx];
}

void ChipEmu::OP_Fx29()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short digit = regV[Vx];

    I = FONTSET_START_ADDRESS + (5 * digit);
}

void ChipEmu::OP_Fx33()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short value = regV[Vx];

    // Ones-place
    memory[I + 2] = value % 10;
    value /= 10;

    // Tens-place
    memory[I + 1] = value % 10;
    value /= 10;

    // Hundreds-place
    memory[I] = value % 10;
}

void ChipEmu::OP_Fx55()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;

    for (unsigned short i = 0; i <= Vx; ++i)
    {
        memory[I + i] = regV[i];
    }
}

void ChipEmu::OP_Fx65()
{
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;

    for (unsigned short i = 0; i <= Vx; ++i)
    {
        regV[i] = memory[I + i];
    }
}