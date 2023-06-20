#pragma once
#include <string>
#include <SDL2/SDL_ttf.h>
#include "function.h"


class Button : public Object {
public:
    Button(std::string msg, SDL_Renderer *screen, int x, int y);
    bool isClicked(Position position);
private:
    std::string text;

};