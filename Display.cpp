#include "Display.h"

Display::Display() : window(nullptr, SDL_DestroyWindow), renderer(nullptr, SDL_DestroyRenderer)
{
    Display::init_SDL();
}

void Display::setPixel(uint8_t row, uint8_t col, bool on)
{
    display[row][col] = on;
}

bool Display::getPixel(uint8_t row, uint8_t col)
{
    return display[row][col];
}

void Display::draw()
{
    if (SDL_RenderClear(renderer.get()) < 0)
    {
        std::cout << "failed to clear renderer: " << SDL_GetError() << "\n";
        exit(1);
    }
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            if (display[row][col] == true)
            {
                SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer.get(), 0x00, 0x00, 0x00, 0x00);
            }
            SDL_RenderFillRect(renderer.get(), rect_from_pixel(row, col).get());
        }
    }
    SDL_RenderPresent(renderer.get());
}

std::unique_ptr<SDL_Rect> Display::rect_from_pixel(int row, int col)
{
    auto r = std::make_unique<SDL_Rect>();
    r->x = col * PIXEL_SCALE;
    r->y = row * PIXEL_SCALE;
    r->w = PIXEL_SCALE;
    r->h = PIXEL_SCALE;
    return r;
}

void Display::init_SDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "failed to initialize SDL\n";
        exit(1);
    }
    window.reset(SDL_CreateWindow(
        "RICK-8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        COLS * PIXEL_SCALE,
        ROWS * PIXEL_SCALE,
        0));

    if (window.get() == nullptr)
    {
        std::cout << "failed to create window\n";
        exit(1);
    }
    renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));

    if (renderer.get() == nullptr)
    {
        std::cout << "failed to create renderer\n";
        exit(1);
    }
}

void Display::destroy_window() {
    SDL_DestroyWindow(window.get());
}