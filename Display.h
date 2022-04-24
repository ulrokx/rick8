#include <stdint.h>
#include <memory>
#include <functional>
#include <SDL2/SDL.h>
#include <iostream>

#ifndef DISPLAY_H
#define DISPLAY_H

#define ROWS 32
#define COLS 64
#define PIXEL_SCALE 10
class Display
{
private:
    bool display[ROWS][COLS] = {{false}};
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;

public:
    Display();
    void setPixel(uint8_t row, uint8_t col, bool on);
    bool getPixel(uint8_t row, uint8_t col);
    void draw();
    void clear();
    void init_SDL();
    std::unique_ptr<SDL_Rect> rect_from_pixel(int row, int col);
};

#endif // DISPLAY_H