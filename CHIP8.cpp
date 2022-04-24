#include "CHIP8.h"

CHIP8::CHIP8() {
    display = std::make_unique<Display>();
    std::fill(RAM, RAM+RAM_SIZE, 0);
}

void CHIP8::load_ROM(char const * filename) {
    std::fstream rom;
    rom.open(filename, std::ios::binary | std::ios::in);
    if(!rom.is_open()) {
        std::cout << "cannot open file\n";
        exit(1);
    }

    rom.seekg(0, std::ios::end);
    ulong length = rom.tellg();
    rom.seekg(0, std::ios::beg);

    auto buf = std::make_unique<char[]>(length);
    rom.read(buf.get(), length);

    for(int i = 0; i < length; ++i) {
        RAM[ROM_START + i] = buf[i];
    }
    rom.close();
    if(rom.is_open()) {
        std::cout << "failed to close file\n";
        exit(1);
    }
}

void CHIP8::print_RAM() {
    for(const uint8_t& b: RAM) {
        std::cout << b;
    }
}

uint16_t CHIP8::fetch()
{
    uint16_t instruction = (static_cast<uint16_t>(RAM[PC]) << 8) | RAM[PC + 1];
    PC += 2;
    return instruction;
}

void CHIP8::decode_and_execute(uint16_t instruction)
{
    uint8_t first_nibble = (instruction >> 12) & 0xF;   // XXXXoooooooooooo
    uint8_t second_nibble = (instruction >> 8) & 0xF;   // ooooXXXXoooooooo
    uint8_t third_nibble = (instruction >> 4) & 0xF;    // ooooooooXXXXoooo
    uint8_t last_byte = (instruction & 0xFF);           // ooooooooXXXXXXXX
    uint16_t last_three_nibbles = instruction & 0x0FFF; // ooooXXXXXXXXXXXX

    switch (first_nibble)
    {
    case 0x0:
    {
        switch (last_byte)
        {
        case 0xEE:
            RET();
            break;
        case 0xE0:
            CLS();
            break;
        }
    default:
        std::cout << "0x0 default case";
        exit(1);
    }
    case 0x1:
    {
        JP(last_three_nibbles);
        break;
    }
    case 0x2:
    {
        break;
    }
    case 0x6:
    {
        LD(second_nibble, last_byte);
        break;
    }
    case 0x7:
        ADD(second_nibble, last_byte);
        break;

    case 0xA:
        LDI(last_three_nibbles);
    }
}

void CHIP8::CLS()
{
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            display->setPixel(row, col, false);
        }
    }
}

void CHIP8::JP(uint16_t address)
{
    PC = address;
}

void CHIP8::LD(uint8_t reg, uint8_t byte)
{
    V[reg] = byte;
}

void CHIP8::ADD(uint8_t reg, uint8_t byte)
{
    V[reg] += byte;
}

void CHIP8::LDI(uint16_t addr)
{
    IC = addr;
}

void CHIP8::DRW(uint8_t x_reg, uint8_t y_reg, uint8_t n)
{
    uint8_t x = V[x_reg] & (COLS - 1);
    uint8_t y = V[y_reg] & (ROWS - 1);
    V[0xF] = 0;
    for (uint8_t i = 0; i < n; ++i)
    {
        uint8_t x = V[x_reg] & (COLS - 1);
        uint8_t data = RAM[IC + i];
        for (uint8_t j = 0; j < 8; ++j)
        {
            bool d = display->getPixel(y, x);
            if ((data >> 7 - j) & 0x1 == 1)
            {
                if (d)
                {
                    display->setPixel(y, x, false);
                    V[0xF] = 0x1;
                }
                else
                {
                    display->setPixel(y, x, true);
                }
            }
            x += 1;
            if (x >= COLS)
            {
                break;
            }
        }
        y += 1;
        if (y >= ROWS)
        {
            break;
        }
    }
}

void CHIP8::RET() {

}
