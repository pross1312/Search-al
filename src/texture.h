#pragma once
#include "utils.h"
#include <SDL2/SDL_image.h>
#include <assert.h>

class Texture {
public:
    Texture() = default;
    virtual ~Texture() { SDL_DestroyTexture(texture); }

    inline int width() const { return w; }
    inline int height() const { return h; }

    inline void loadImage(const char *path, SDL_Renderer *renderer, int rKey, int gKey, int bKey) {
        assert(renderer != nullptr);
        SDL_Surface *surface = check(IMG_Load(path));

        // transparent background
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, rKey, gKey, bKey));

        clip.w = surface->w;
        clip.h = surface->h;
        w = surface->w;
        h = surface->h;
        texture = check(SDL_CreateTextureFromSurface(renderer, surface));
        SDL_FreeSurface(surface);
    }

    inline void draw(SDL_Renderer* renderer, int x, int y) {
        assert(renderer != nullptr);
        SDL_Rect dst {
            .x = x,
            .y = y,
            .w = clip.w,
            .h = clip.h
        };
        SDL_RenderCopy(renderer, texture, &clip, &dst);
    }
    inline void draw(SDL_Renderer* renderer, SDL_Rect dst) {
        assert(renderer != nullptr);
        SDL_RenderCopy(renderer, texture, &clip, &dst);
    }

protected:
    int w, h;
    SDL_Rect clip{ 0, 0, 0, 0 };
    SDL_Texture* texture = nullptr;
};
