#include <stdint.h>
#include <iostream>
#ifndef KEYPAD_H
#define KEYPAD_H
#define KEYCOUNT 16

class Keypad {
    private:
    bool KEYS[KEYCOUNT] = {false};
    public:
    bool getKey(uint8_t key);
    bool isPressed();
    void updateKeypad();
};

#endif