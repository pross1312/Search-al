#include <fcntl.h>
#include "function.h"
#include "Button.h"
#include "Grid.h"
#include "Findpath.h"

const int ROWS = 30;
const int COLUMNS = 30;
Position startPos = { 0, 0 };
Position endPos = { ROWS-5, COLUMNS-5 };

SDL_Window* g_window = NULL;
SDL_Renderer* g_screen = NULL;
SDL_Event g_event;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("ERROR: Couldn't initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    if (TTF_Init() != 0)
        return false;
    g_window = SDL_CreateWindow("PATH",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        COLUMNS * Cell::SIZE,
        ROWS * Cell::SIZE + 30,
        SDL_WINDOW_SHOWN);
    if (g_window == NULL) {
        SDL_Log("ERROR: Couldn't create window: %s\n", SDL_GetError());
        return false;
    }
    g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (g_screen == NULL) {
        SDL_Log("ERROR: Couldn't create renderer: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

int main(void)
{
    if (init() == false)
        exit(1);

    Grid grid{ ROWS, COLUMNS, g_screen };
    grid.draw(g_screen);

    Button write{ "Write", g_screen, 5, ROWS * Cell::SIZE };
    Button read{ "Read", g_screen, write.getWidth() + 30, ROWS * Cell::SIZE };
    Button start{ "Start", g_screen, 150, ROWS * Cell::SIZE };
    Button restart{ "Restart", g_screen, 200 + start.getWidth(), ROWS * Cell::SIZE };


    grid.getCell(startPos).setState(Corrected);
    grid.getCell(endPos).setState(Corrected);

    bool quit = false;
    bool on_left_mouse_click = false;
    bool on_right_mouse_click = false;
    while (!quit) {
        Position mouseClick;
        SDL_GetMouseState(&mouseClick.x, &mouseClick.y);
        while (SDL_PollEvent(&g_event) != 0) {
            if (g_event.type == SDL_QUIT)
                quit = true;
            else if (g_event.type == SDL_MOUSEBUTTONDOWN) {
                if (g_event.button.button == SDL_BUTTON_RIGHT) {
                    on_right_mouse_click = true;
                    on_left_mouse_click = false;
                }
                else if (g_event.button.button == SDL_BUTTON_LEFT) {
                    if (write.isClicked(mouseClick))
                        grid.saveToFile("map\\map01.txt");
                    else if (read.isClicked(mouseClick))
                        grid.readFromFile("map\\map01.txt");
                    else if (start.isClicked(mouseClick)) {
                        Findpath f{ &grid, startPos, endPos };
                        if (f.find() == false)
                            Error("WHAT A FOOLLLLLL!\n");
                        else
                            f.drawCorrectedPath();
                    }
                    else if (restart.isClicked(mouseClick)) {
                        grid.clear();
                        grid.getCell(startPos).setState(Corrected);
                        grid.getCell(endPos).setState(Corrected);
                    }
                    else {
                        on_left_mouse_click = true;
                        on_right_mouse_click = false;
                        Position cell = grid.pointToGridCell(mouseClick);
                        if (grid.isValidPosition(cell))
                            grid.setCellWalkable(cell, !grid.getCell(cell).getWalkable());
                    }
                }
            }
            else if (g_event.type == SDL_MOUSEBUTTONUP) {
                if (g_event.button.button == SDL_BUTTON_LEFT) on_left_mouse_click = false;
                else if (g_event.button.button == SDL_BUTTON_RIGHT) on_right_mouse_click = false;
            }
        }
        if (on_left_mouse_click) {
            Position cell = grid.pointToGridCell(mouseClick);
            if (grid.isValidPosition(cell))
                grid.setCellWalkable(cell, false);
        }
        else if (on_right_mouse_click) { 
            Position cell = grid.pointToGridCell(mouseClick);
            if (grid.isValidPosition(cell))
                grid.setCellWalkable(cell, true);
        }
        start.draw(g_screen, NULL);
        read.draw(g_screen, NULL);
        write.draw(g_screen, NULL);
        restart.draw(g_screen, NULL);
        grid.draw(g_screen);
        SDL_RenderPresent(g_screen);
    }

    return 0;
}

