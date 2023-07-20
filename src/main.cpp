#include "Button.h"
#include "Grid.h"
#include "widget-manager.h"
#include "a-star.h"

// Vec2i startPos = { 0, 0 };
// Vec2i endPos = { ROWS-5, COLUMNS-5 };

size_t S_WIDTH = 800;
size_t S_HEIGHT = 700;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

int main(void)
{
    WidgetManager widgetManager(SDL_Rect{ .x = 0, .y = 0, .w = int(S_WIDTH), .h = int(S_HEIGHT) }, PortionHorizontal, {.8f, .2f});

    const int rows = 30;
    const int columns = 30;
    Grid grid(rows, columns);
    widgetManager.add(&grid);

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
    bool dirty = false;

    const char* fontPath = "iosevka-term-regular.ttf";
    TTF_Font* font = check(TTF_OpenFont(fontPath, 24));

    WidgetManager buttonsManager(GridLayout, 4, 1);
    widgetManager.add(&buttonsManager);
    Button buttons[] = {
        Button("Reset", font, renderer, [&]() { grid.clear(); dirty = false; }),
        Button("Clear path", font, renderer, [&]() { grid.clearPath(); dirty = false; }),
        Button("AStart", font, renderer, [&]() {
            if (dirty) return;
            AStarFinder finder;
            finder.find(&grid, Vec2i{0, 0}, Vec2i{(int)grid.MAXCOLUMNS-1, (int)grid.MAXROWS-1});
            dirty = true;
        }),
    };
    for (auto& b : buttons) buttonsManager.add(&b);

    while (!quit) {
        Vec2i mousePos;
        SDL_GetMouseState(&mousePos.x, &mousePos.y);
        while (SDL_PollEvent(&event) != 0) {
            for (auto& b : buttons) b.update(event, mousePos);
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    S_WIDTH = event.window.data1;
                    S_HEIGHT = event.window.data2;
                    widgetManager.setBound(SDL_Rect{ .x = 0, .y = 0, .w = int(S_WIDTH), .h = int(S_HEIGHT)});
                }
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
            }
        }
        if (on_left_mouse_click || on_right_mouse_click) {
            Vec2i cell = grid.pointToGridCell(mousePos);
            if (grid.isValidPosition(cell)) grid.at(cell) = on_left_mouse_click ? Unwalkable : Walkable;
        }
        check(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255));
        check(SDL_RenderClear(renderer));
        widgetManager.draw(renderer);
        SDL_RenderPresent(renderer);
    }
    return 0;
}
