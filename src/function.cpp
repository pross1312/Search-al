#include "function.h"

void Object::draw(SDL_Renderer *dst, SDL_Rect *clip)
{
    SDL_RenderCopy(dst, texture, clip, &rectDst);
}

bool Object::loadImage(const char *path, SDL_Renderer *screen)
{
    if (screen == nullptr)
        return false;
    // load image
    SDL_Surface *surface = IMG_Load(path);
    if (surface == nullptr)
    {
        Error("Load image error");
        return false;
    }
    // transparent background
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, REDKEY, GREENKEY, BLUEKEY));
    rectDst.w = surface->w;
    rectDst.h = surface->h;
    // get texture object
    texture = SDL_CreateTextureFromSurface(screen, surface);
    // delete surface
    SDL_FreeSurface(surface);
    if (texture == nullptr)
    {
        Error("Can't create texture from image");
        return false;
    }
    return true;
}



int Position::getDistance(const Position& a) {
    Position tmp = *this - a;
    int distance = 100 * sqrt(tmp.x*tmp.x + tmp.y*tmp.y);
    return distance;
}