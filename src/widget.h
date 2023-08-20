#pragma once
#include <SDL2/SDL.h>

class Widget {
public:
    Widget() = default;
    Widget(SDL_Rect b): bound(b) {}
    virtual void draw(SDL_Renderer* renderer) = 0;
    virtual ~Widget() = default;
    virtual void set_bound(SDL_Rect b) {
        bound = b;
    }
    SDL_Rect bound;
};
