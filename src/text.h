#pragma once
#include "widget.h"
#include <string>
#include <SDL2/SDL_ttf.h>

struct Text : public Widget {
    Text(const char* text, TTF_Font* font, SDL_Renderer* renderer, uint32_t color, int x, int y);
    void draw(SDL_Renderer* renderer) override;
    void set_bound(SDL_Rect b) override;
    void set_text(SDL_Renderer* renderer, const char* text);
//    void set_color(uint32_t color);

    TTF_Font* font;
    SDL_Color color;
    std::string text;
    SDL_Texture* texture;
};
