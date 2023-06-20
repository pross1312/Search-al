#include "Button.h"
Button::Button(std::string msg, SDL_Renderer* screen, int x, int y) : Object{ x, y }, text{ msg } {

    TTF_Font* Sans = TTF_OpenFont("resources/iosevka-regular.ttf", 24);
    if (Sans == NULL)
        Error(TTF_GetError());
    SDL_Color Black = { 0, 0, 0, 255};


    SDL_Surface* s = TTF_RenderText_Solid(Sans, msg.c_str(), Black);
    if (s == NULL)
        Error(SDL_GetError());
    rectDst.w = s->w;
    rectDst.h = s->h;

    SDL_Surface* background = SDL_CreateRGBSurface(0, s->w, s->h, 32, 0, 0, 0, 0);
    if (background == NULL)
        Error(SDL_GetError());

    SDL_FillRect(background, NULL, SDL_MapRGB(background->format, 255, 0, 0));
    SDL_BlitSurface(s, NULL, background, NULL);

    texture = SDL_CreateTextureFromSurface(screen, background);
    if (texture == NULL)
        Error(SDL_GetError());

    SDL_FreeSurface(background);
    SDL_FreeSurface(s);
    TTF_CloseFont(Sans);
}

bool Button::isClicked(Position pos) {
    if (pos.y >= rectDst.y && pos.y < rectDst.y + rectDst.h &&
        pos.x >= rectDst.x && pos.x < rectDst.x + rectDst.w)
        return true;
    return false;
}
