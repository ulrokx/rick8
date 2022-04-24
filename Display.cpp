#include "Display.h"

Display::Display() {
    for(uint8_t r; r < ROWS; ++r) {
        for(uint8_t c; c< COLS; ++c) {
            display[r][c] = false;
        }
    }
}

void Display::setPixel(uint8_t row, uint8_t col, bool on) {
    display[row][col] = on;
}

bool Display::getPixel(uint8_t row, uint8_t col) {
    return display[row][col];
}