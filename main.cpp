#include "CHIP8.h"
int main(int argc, char** argv) {
    auto chip8 = std::make_unique<CHIP8>();

    if(argc != 2) {
        std::cout << "usage: emu filename\n";
        exit(1);
    }
    chip8->load_ROM(argv[1]);
    chip8->print_RAM();
}