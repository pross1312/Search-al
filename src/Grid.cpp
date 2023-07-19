#include <fstream>
#include "Grid.h"

Grid::Grid(size_t rows, size_t cols) : MAXROWS{ rows }, MAXCOLUMNS{ cols }, position{} {
    grid = new State [MAXROWS * MAXCOLUMNS];
    memset(grid, 0, MAXROWS * MAXCOLUMNS * sizeof(grid[0]));
}

void Grid::saveToFile(const char* fName) {
    std::ofstream fout(fName, std::ios::out);
    if (!fout)
        throw "Can't open file to save";
    for (size_t i = 0; i < MAXROWS; i++) {
        for (size_t j = 0; j < MAXCOLUMNS; j++) {
            int state = at(i, j) == Unwalkable ? Unwalkable : Walkable;
            fout << state << " ";
        }
    }
    fout.close();
}

void Grid::readFromFile(const char* fName) {
    std::ifstream fin(fName, std::ios::in);
    if (!fin)
        throw "Can't open file to read";
    for (size_t i = 0; i < MAXROWS; i++)
        for (size_t j = 0; j < MAXROWS; j++) {
            int w = false;
            fin >> w;
            at(i, j) = (State)w;
        }
    fin.close();
}

void Grid::draw(SDL_Renderer* renderer) {
    check(SDL_SetRenderDrawColor(renderer, UNHEX(uint8_t, OUTLINE_COLOR)));
    const SDL_Rect outline {
        .x = position.x,
        .y = position.y,
        .w = (int)pxWidth(),
        .h = (int)pxHeight(),
    };
    check(SDL_RenderFillRect(renderer, &outline));
    for (size_t i = 0; i < MAXROWS; i++) {
        for (size_t j = 0; j < MAXCOLUMNS; j++) {
            const SDL_Rect rect {
                .x = (int)(j * CELLSIZE + PADDING + position.x),
                .y = (int)(i * CELLSIZE + PADDING + position.y),
                .w = (int)(CELLSIZE - 2 * PADDING),
                .h = (int)(CELLSIZE - 2 * PADDING),
            };
            check(SDL_SetRenderDrawColor(renderer, UNHEX(uint8_t, stateColor[at(i, j)])));
            check(SDL_RenderFillRect(renderer,
                    &rect));
        }
    }
}

Vec2i Grid::pointToGridCell(Vec2i pos) {
    pos = pos - position;
    pos.x /= CELLSIZE;
    pos.y /= CELLSIZE;
    return pos;
}

void Grid::clear() {
    for (size_t i = 0; i < MAXROWS; i++) {
        for (size_t j = 0; j < MAXCOLUMNS; j++) {
            at(i, j) = Walkable;
        }
    }
}
