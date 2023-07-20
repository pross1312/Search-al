#pragma once
#include <SDL2/SDL.h>

#define UNHEX(type, c) (type)((uint8_t)((c) >> 8 * 3) & (uint8_t)0xff)\
                      ,(type)((uint8_t)((c) >> 8 * 2) & (uint8_t)0xff)\
                      ,(type)((uint8_t)((c) >> 8)     & (uint8_t)0xff)\
                      ,(type)((uint8_t)((c))          & (uint8_t)0xff)

template<typename T>
inline T* check(T* data) {
    if (data == nullptr) {
        SDL_Log("ERROR: %s\n", SDL_GetError());
        exit(1);
    }
    return data;
}
inline void check(int data) {
    if (data < 0) {
        SDL_Log("ERROR: %s\n", SDL_GetError());
        exit(1);
    }
}
