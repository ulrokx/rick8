#include <stdint.h>

#ifndef DISPLAY_H
#define DISPLAY_H

#define ROWS 32
#define COLS 64
class Display
{
private:
    bool display[ROWS][COLS];

public:
    Display();
    void setPixel(uint8_t row, uint8_t col, bool on);
    bool getPixel(uint8_t row, uint8_t col);
    void draw();
    void clear();
};

#endif // DISPLAY_H