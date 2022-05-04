#include "Keypad.h"

bool Keypad::getKey(uint8_t key) {
    if(key > 0xF) {
        std::cout << "key out of range\n";
        exit(1);
    }
    return KEYS[key];
}

bool Keypad::isPressed() {
    for(int i = 0x0; i < KEYCOUNT; ++i) {
        if(KEYS[i] == true) {
            return true;
        }
    }
    return false;
}


uint8_t Keypad::handleEvents() {
    uint8_t newKey = 0xEE;
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            return 0xFF;
        }
        bool up = e.type == SDL_KEYUP;
        switch(e.key.keysym.sym) {
            case SDLK_1:
                newKey = toggle(0x1, up);
                break;
            case SDLK_2:
                newKey = toggle(0x2, up);
                break;
            case SDLK_3:
                newKey = toggle(0x3, up);
                break;
            case SDLK_4:
                newKey = toggle(0xC, up);
                break;
            case SDLK_q:
                newKey = toggle(0x4, up);
                break;
            case SDLK_w:
                newKey = toggle(0x5, up);
                break;
            case SDLK_e:
                newKey = toggle(0x6, up);
                break;
            case SDLK_r:
                newKey = toggle(0xD, up);
                break;
            case SDLK_a:
                newKey = toggle(0x7, up);
                break;
            case SDLK_s:
                newKey = toggle(0x8, up);
                break;
            case SDLK_d:
                newKey = toggle(0x9, up);
                break;
            case SDLK_f:
                newKey = toggle(0xE, up);
                break;
            case SDLK_z:
                newKey = toggle(0xA, up);
                break;
            case SDLK_x:
                newKey = toggle(0x0, up);
                break;
            case SDLK_c:
                newKey = toggle(0xB, up);
                break;
            case SDLK_v:
                newKey = toggle(0xF, up);
                break;
        }
    }
    return newKey;
}

uint8_t Keypad::toggle(uint8_t key, bool up) {
    if(up) {
        KEYS[key] = false;
    } else {
        KEYS[key] = true;
    }
    return key;
}