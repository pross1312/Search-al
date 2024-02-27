#include "text.h"
#include "utils.h"

Text::Text(const char* msg, TTF_Font* font, SDL_Renderer* renderer, uint32_t color, int x, int y):
    font{ font },
    color{ UNHEX(uint8_t, color) },
    text{ msg } {
        SDL_Surface* text_surface = check(TTF_RenderText_Solid(
                                            font,
                                            msg,
                                            this->color));
        set_bound(SDL_Rect{ .x = x, .y = y, .w = text_surface->w, .h = text_surface->h });
        texture = check(SDL_CreateTextureFromSurface(renderer, text_surface));
        SDL_FreeSurface(text_surface);
}

void Text::draw(SDL_Renderer* renderer) {
    check(SDL_RenderCopy(renderer, texture, nullptr, &bound));
}

void Text::set_bound(SDL_Rect b) {
    Widget::set_bound(b);
}

void Text::set_text(SDL_Renderer* renderer, const char* text) {
    this->text = text;
    SDL_Surface* text_surface = check(TTF_RenderText_Solid(
                                        font,
                                        text,
                                        this->color));
    texture = check(SDL_CreateTextureFromSurface(renderer, text_surface));
    SDL_FreeSurface(text_surface);
}
//void Text::set_color(uint32_t color) {
//    SDL_Surface* text_surface = check(TTF_RenderText_Solid(
//                font,
//                text.c_str(),
//                SDL_Color{UNHEX(uint8_t, color)}));
//    SDL_DestroyTexture(texture);
//    texture = check(SDL_CreateTextureFromSurface(renderer, text));
//    SDL_FreeSurface(text);
//}
