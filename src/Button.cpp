#include "Button.h"
#include "utils.h"

template<typename T>
T* check(T* data);
void check(int data);

Button::Button(const char* msg, TTF_Font* font, SDL_Renderer* screen, int x, int y): text{ msg } {

    SDL_Surface* s = check(TTF_RenderText_Solid(font,  msg, SDL_Color{ 0, 0, 0, 255 }));

    bound.x = x;
    bound.y = y;
    bound.w = s->w;
    bound.h = s->h;

    SDL_Surface* background = check(SDL_CreateRGBSurface(0, s->w, s->h, 32, 0, 0, 0, 0));
    check(SDL_FillRect(background, NULL, SDL_MapRGB(background->format, 255, 0, 0)));
    check(SDL_BlitSurface(s, NULL, background, NULL));

    texture = check(SDL_CreateTextureFromSurface(screen, background));

    SDL_FreeSurface(background);
    SDL_FreeSurface(s);
}

bool Button::isClicked(Vec2i pos) {
    if (pos.y >= bound.y && pos.y < bound.y + bound.h &&
        pos.x >= bound.x && pos.x < bound.x + bound.w)
        return true;
    return false;
}
