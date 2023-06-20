#include "Cell.h"

bool Cell::initGraphic(SDL_Renderer *screen) {
    bool success = true;
    success |= obj[0].loadImage("resources/Unselected.png", screen);
    success |= obj[1].loadImage("resources/Selected.png", screen);
    success |= obj[2].loadImage("resources/Corrected.png", screen);
    return success;
}
void Cell::draw(Position position, SDL_Renderer *screen) {
    obj[state].setPosition(position.x, position.y);
    if (state == Unselected) {
        SDL_Rect clip;
        clip.x = 0;
        clip.y = 0;
        clip.w = SIZE;
        clip.h = SIZE;
        if (!walkable) {
            clip.x = SIZE;
            obj[state].draw(screen, &clip);
        }
        else
            obj[state].draw(screen, &clip);
    }
    else {
        obj[state].draw(screen, NULL);
    }
}
