#include "Button.h"
#include "Grid.h"
#include "a-star.h"

const int ROWS = 30;
const int COLUMNS = 30;
// Vec2i startPos = { 0, 0 };
// Vec2i endPos = { ROWS-5, COLUMNS-5 };


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

int main(void)
{
    Grid grid(ROWS, COLUMNS);
    grid.position = {50, 50};

    size_t S_WIDTH = grid.pxWidth() + 100;
    size_t S_HEIGHT = grid.pxHeight() + 100;
    check(SDL_Init(SDL_INIT_VIDEO));
    check(TTF_Init());
    window = check(SDL_CreateWindow("PATH",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    S_WIDTH,
                    S_HEIGHT,
                    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));
    renderer = check(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

    bool quit = false;
    bool on_left_mouse_click = false;
    bool on_right_mouse_click = false;
    AStarFinder finder;
    while (!quit) {
        Vec2i mousePos;
        SDL_GetMouseState(&mousePos.x, &mousePos.y);
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_WINDOWEVENT:
                  break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    on_right_mouse_click = true;
                    on_left_mouse_click = false;
                }
                else if (event.button.button == SDL_BUTTON_LEFT) {
                    on_left_mouse_click = true;
                    on_right_mouse_click = false;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) on_left_mouse_click = false;
                else if (event.button.button == SDL_BUTTON_RIGHT) on_right_mouse_click = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_a) {
                    finder.find(&grid, Vec2i{0, 0}, Vec2i{(int)grid.MAXCOLUMNS-1, (int)grid.MAXROWS-1});
                }
                break;
            }
        }
        if (on_left_mouse_click || on_right_mouse_click) {
            Vec2i cell = grid.pointToGridCell(mousePos);
            if (grid.isValidPosition(cell)) grid.at(cell) = on_left_mouse_click ? Unwalkable : Walkable;
        }
        check(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255));
        check(SDL_RenderClear(renderer));
        grid.draw(renderer);
        SDL_RenderPresent(renderer);
    }
    return 0;
}
