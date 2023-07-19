#pragma once
#include <string>
#include <SDL2/SDL_ttf.h>
#include "vec2.h"

class Button {
public:
    Button(const char* msg, TTF_Font* font, SDL_Renderer *screen, int x, int y);
    bool isClicked(Vec2i position);
private:
    std::string text;
    SDL_Rect bound;
    SDL_Texture* texture;
};
