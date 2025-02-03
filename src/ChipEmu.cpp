#include<fstream>
#include<chrono>
#include<cstring>

#include"ChipEmu.h"


const unsigned int VIDEO_WIDTH = 64;
const unsigned int VIDEO_HEIGHT = 32;

void ChipEmu::cycle(){
    opcode = (memory[PC] << 8U) | memory[PC + 1];

    PC += 2;

    ((*this).*(table[(opcode & 0xF000U) >> 12U]))();

    if(delayTimer > 0){
        --delayTimer;
    }
    if(soundTimer > 0){
        --soundTimer;
    }
}

void ChipEmu::loadROM(char const* filename){
    
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if(file.is_open()){
        std::streampos size = file.tellg();
        char* buffer = new char[size];

        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        for(long i = 0; i < size; ++i){
            memory[START_ADDRESS + i] = buffer[i];
        }

        delete[] buffer;
    }
}

void ChipEmu::OP_00E0(){
    memset(video, 0, sizeof(video));
}

void ChipEmu::OP_00EE(){
    --SP;
    PC = stack[SP];
}

void ChipEmu::OP_1nnn(){
    unsigned short address = opcode & 0x0FFFU;
    PC = address;
}

void ChipEmu::OP_2nnn(){
    unsigned short address = opcode & 0x0FFFU;
    stack[SP] = PC;
    ++SP;
    PC = address;
}

void ChipEmu::OP_3xkk(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short byte = opcode & 0x00FFU;

    if(regV[Vx] == byte){
        PC += 2;
    }
}

void ChipEmu::OP_4xkk(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short byte = opcode & 0x00FFU;

    if(regV[Vx] != byte){
        PC += 2;
    }
}


void ChipEmu::OP_5xy0(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    if(regV[Vx] == regV[Vy]){
        PC += 2;
    }
}

void ChipEmu::OP_6xkk(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short byte = opcode & 0x00FFU;

    regV[Vx] = byte;
}

void ChipEmu::OP_7xkk(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short byte = opcode & 0x00FFU;

    regV[Vx] += byte;
}

void ChipEmu::OP_8xy0(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    regV[Vx] = regV[Vy];
}

void ChipEmu::OP_8xy1(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    regV[Vx] |= regV[Vy];
}

void ChipEmu::OP_8xy2(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    regV[Vx] &= regV[Vy];
}

void ChipEmu::OP_8xy3(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    regV[Vx] ^= regV[Vy];
}

void ChipEmu::OP_8xy4(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    unsigned short sum = regV[Vx] + regV[Vy];

    if(sum > 255U){
        regV[0xF] = 1;
    }else{
        regV[0xF] = 0;
    }

    regV[Vx] = sum & 0xFFU;
}

void ChipEmu::OP_8xy5(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    if(regV[Vx] > regV[Vy]){
        regV[0xF] = 1;
    }else{
        regV[0xF] = 0;
    }

    regV[Vx] -= regV[Vy];
}

void ChipEmu::OP_8xy6(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;

    regV[0xF] = (regV[Vx] & 0x1U);

    regV[Vx] >>= 1;
}

void ChipEmu::OP_8xy7(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    if(regV[Vy] > regV[Vx]){
        regV[0xF] = 1;
    }else{
        regV[0xF] = 0;
    }

    regV[Vx] = regV[Vy] - regV[Vx];
}

void ChipEmu::OP_8xyE(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;

    // Save MSB in VF
	regV[0xF] = (regV[Vx] & 0x80u) >> 7u;

	regV[Vx] <<= 1;
}

void ChipEmu::OP_9xy0(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;

    if(regV[Vx] != regV[Vy]){
        PC += 2;
    }
}

void ChipEmu::OP_Annn(){
    I = opcode & 0x0FFFU;
}

void ChipEmu::OP_Bnnn(){
    PC = regV[0] & 0x0FFFU;
}

void ChipEmu::OP_Cxkk(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short byte = opcode & 0x00FFU;

    regV[Vx] = randByte(randGen) & byte;
}

void ChipEmu::OP_Dxyn(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short Vy = (opcode & 0x00F0U) >> 4U;
    unsigned short height = opcode & 0x000FU;

    unsigned short xPos = regV[Vx] % VIDEO_WIDTH;
    unsigned short yPos = regV[Vy] % VIDEO_HEIGHT;

    regV[0xF] = 0;

    for(unsigned int row = 0; row < height; ++row){
        unsigned short spriteByte = memory[I + row];

        for(unsigned int col = 0; col < 8; ++col){
            unsigned short spritePixel = spriteByte & (0x80U >> col);
            unsigned long* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

            if(spritePixel){
                if(*screenPixel == 0xFFFFFFFF){
                    regV[0xF] = 1;
                }
                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void ChipEmu::OP_Ex9E(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short key = regV[Vx];

    if(keyPad[key]){
        PC += 2;
    }
}

void ChipEmu::OP_ExA1(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short key = regV[Vx];

    if(!keyPad[key]){
        PC += 2;
    }
}

void ChipEmu::OP_Fx07(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;

    regV[Vx] = delayTimer;
}

void ChipEmu::OP_Fx0A(){
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

void ChipEmu::OP_Fx15(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    delayTimer = regV[Vx];
}

void ChipEmu::OP_Fx18(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    soundTimer = regV[Vx];
}

void ChipEmu::OP_Fx1E(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    I += regV[Vx];
}

void ChipEmu::OP_Fx29(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;
    unsigned short digit = regV[Vx];

    I = FONTSET_START_ADDRESS + (5 * digit);
}

void ChipEmu::OP_Fx33(){
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

void ChipEmu::OP_Fx55(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;

    for(unsigned short i = 0; i <= Vx; ++i){
        memory[I + i] = regV[i];
    }
}

void ChipEmu::OP_Fx65(){
    unsigned short Vx = (opcode & 0x0F00U) >> 8U;

    for(unsigned short i = 0; i <= Vx; ++i){
        regV[i] = memory[I + i];
    }
}