#pragma once
#include <SDL2/SDL.h>

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
