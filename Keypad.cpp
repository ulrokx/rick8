#include "Keypad.h"

bool Keypad::getKey(uint8_t key) {
    if(key > 0xF) {
        std::cout << "key out of range\n";
        exit(1);
    }
    return KEYS[key];
}

bool Keypad::isPressed() {
    for(int i = 0x00; i < KEYCOUNT; ++i) {
        if(KEYS[i] == true) {
            return true;
        }
    }
    return false;
}

void Keypad::updateKeypad() {
    std::cout << "not implemented\n";
}