#pragma once
#include <SDL2/SDL.h>

class Widget {
public:
    Widget() = default;
    Widget(SDL_Rect b): bound(b) {}
    virtual void draw(SDL_Renderer* renderer) = 0;
    virtual ~Widget() = default;
    virtual inline void setBound(SDL_Rect b) {
        bound = b;
    }
protected:
    SDL_Rect bound;
};
