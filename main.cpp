#include "CHIP8.h"
#include <cstdlib>
#include <iostream>
bool debug = false;
long fps = 60;
uint64_t tpf = 1000 / fps;

void handleArguments(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "usage: emu rom fps\n";
        exit(1);
    }
    debug = false;
    if (argc == 4 && argv[3][0] == 'd')
    {
        debug = true;
    }
    fps = atol(argv[2]);
    if (fps == 0)
    {
        std::cout << "fps is not a number or 0\n";
        exit(1);
    }
    tpf = 1000 / fps;
    if (tpf <= 1)
    {
        std::cout << "fps is too high\n";
        exit(1);
    }
}
int main(int argc, char **argv)
{
    handleArguments(argc, argv);
    auto chip8 = std::make_unique<CHIP8>(debug);
    chip8->load_ROM(argv[1]);
    uint64_t t;
    while (true)
    {
        if (SDL_GetTicks() - t > tpf)
        {
            t = SDL_GetTicks();
            chip8->step();
        }
    }
}