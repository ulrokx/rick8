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
#define FONTSET_SIZE 0x50
#define FONTSET_START 0x50

class CHIP8
{
private:
    uint8_t FONTSET[FONTSET_SIZE] = {
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
    void JP(uint16_t addr);                            // 1NNN jump to NNN
    void CALL(uint16_t addr);                          // 2NNN call subroutine at NNN, incr sp, put pc on stack
    void SE(uint8_t reg, uint8_t byte);                // 3xkk if Vx is equal to kk, incr pc by 2
    void SNE(uint8_t reg, uint8_t byte);               // 4xkk if Vx not equal to kk, incr pc by 2
    void SER(uint8_t x_reg, uint8_t y_reg);            // 5xy0 if Vx equal to Vy, incr pc by 2
    void LD(uint8_t reg, uint8_t byte);                // 6xkk load kk into register x
    void ADD(uint8_t reg, uint8_t byte);               // 7xkk add kk to register x
    void LDR(uint8_t x_reg, uint8_t y_reg);            // 8xy0 store vy in vx
    void OR(uint8_t x_reg, uint8_t y_reg);             // 8xy1 OR vx and vy - store in vx
    void AND(uint8_t x_reg, uint8_t y_reg);            // 8xy2 AND vx and vy - store in vx
    void XOR(uint8_t x_reg, uint8_t y_reg);            // 8xy3 XOR vx and vy - store in vx
    void ADDC(uint8_t x_reg, uint8_t y_reg);           // 8xy4 add vx and vy - store in vx, > 255, VF = 1
    void SUB(uint8_t x_reg, uint8_t y_reg);            // 8xy5 sub vx and vy - store in vx !!!
    void SHR(uint8_t x_reg, uint8_t y_reg);            // 8xy6
    void SUBN(uint8_t x_reg, uint8_t y_reg);           // 8xy7
    void SHL(uint8_t x_reg, uint8_t y_reg);            // 8xyE
    void SNE(uint8_t x_reg, uint8_t y_reg);            // 9xy0 if vx != vy incr pc by 2
    void LDI(uint16_t addr);                           // Annn set IC to nnn
    void JPP(uint16_t addr);                           // Bnnn jump to nnn + v0
    void RND(uint8_t reg, uint8_t byte);               // Cxkk gen rand 0-255 and with kk - store vx
    void DRW(uint8_t x_reg, uint8_t y_reg, uint8_t n); // DXYN draw
    void SKP(uint8_t reg);                             // Ex9E if vx is pressed, incr pc by 2
    void SKNP(uint8_t reg);                            // ExA1 if vx is not pressed, incr pc by 2
    void LDT(uint8_t reg);                             // Fx07 timer put in Vx
    void LDK(uint8_t reg);                             // Fx0A wait for key, store in Vx
    void LDDT(uint8_t reg);                            // Fx15 set delay timer from vx
    void LDST(uint8_t reg);                            // Fx18 set sound timer from vx
    void ADDI(uint8_t reg);                            // Fx1E I += Vx
    void LDF(uint8_t reg);                             // Fx29 set I to hex font in Vx
    void LDB(uint8_t reg);                             // Fx33
    void RTM(uint8_t reg);                             // Fx55 store registers in memory from IC
    void MTR(uint8_t reg);                             // Fx65 store memory to registers from I
public:
    uint16_t fetch();
    void decode_and_execute(uint16_t instruction);
    void load_ROM(char const *filename);
    void print_RAM();
    CHIP8();
};
#endif // CHIP8_H