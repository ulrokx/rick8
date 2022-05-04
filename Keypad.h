#include <stdint.h>
#include <iostream>
#include <SDL2/SDL_events.h>
#ifndef KEYPAD_H
#define KEYPAD_H
#define KEYCOUNT 16

class Keypad {
    private:
    bool KEYS[KEYCOUNT] = {false};
    SDL_Event e;
    uint8_t toggle(uint8_t key, bool up);
    public:
    bool getKey(uint8_t key);
    bool isPressed();
    void updateKeypad();
    uint8_t handleEvents();
};

#endif