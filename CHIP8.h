#include <iostream>
#include <stdint.h>
#include <memory>
#include <fstream>
#include "Display.h"

#ifndef CHIP8_H
#define CHIP8_H

#define NNN 0x0FFF
#define ROM_START 0x200
#define RAM_SIZE 0x1000
#define REGISTER_COUNT 16
#define STACK_HEIGHT 16
class CHIP8
{
private:
    // memory
    uint8_t RAM[RAM_SIZE];
    // program counter
    uint16_t PC;
    // index counter
    uint16_t IC;
    // registers
    uint8_t V[REGISTER_COUNT];
    // stack
    uint16_t STACK[STACK_HEIGHT];
    // stack pointer
    uint8_t SP;
    std::unique_ptr<Display> display;

    // display

    void CLS();                                        // 00E0 clear the display
    void RET();                                        // 00EE return
    void JP(uint16_t instruction);                     //  1NNN jump to NNN
    void LD(uint8_t reg, uint8_t byte);                // 6xkk load kk into register x
    void ADD(uint8_t reg, uint8_t byte);               // 7xkk add kk to register x
    void LDI(uint16_t addr);                           // Annn set IC to nnn
    void DRW(uint8_t x_reg, uint8_t y_reg, uint8_t n); // DXYN draw
public:
    uint16_t fetch();
    void decode_and_execute(uint16_t instruction);
    void load_ROM(char const * filename);
    void print_RAM();
    CHIP8();
};
#endif // CHIP8_H