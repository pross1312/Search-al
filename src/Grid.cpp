#include <fstream>
#include "Grid.h"

Grid::Grid(size_t rows, size_t cols) : MAXROWS{ rows }, MAXCOLUMNS{ cols }, position{} {
    grid = new State [MAXROWS * MAXCOLUMNS];
    memset(grid, 0, MAXROWS * MAXCOLUMNS * sizeof(grid[0]));
}

void Grid::save(const char* fName) {
    std::ofstream fout(fName, std::ios::out);
    if (!fout)
        throw "Can't open file to save";
    for (size_t i = 0; i < MAXROWS * MAXCOLUMNS; i++) {
        int state = grid[i] == Unwalkable ? Unwalkable : Walkable;
        fout << state << " ";
    }
    fout.close();
}

void Grid::load(const char* fName) {
    std::ifstream fin(fName, std::ios::in);
    if (!fin)
        throw "Can't open file to read";
    for (size_t i = 0; i < MAXROWS * MAXCOLUMNS; i++) {
        int w = false;
        fin >> w;
        grid[i] = (State)w;
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
                .x = (int)(j * cellSize + PADDING + position.x),
                .y = (int)(i * cellSize + PADDING + position.y),
                .w = (int)(cellSize - 2 * PADDING),
                .h = (int)(cellSize - 2 * PADDING),
            };
            check(SDL_SetRenderDrawColor(renderer, UNHEX(uint8_t, STATE_COLOR[at(i, j)])));
            check(SDL_RenderFillRect(renderer,
                    &rect));
        }
    }
}

Vec2i Grid::point_to_cell(Vec2i pos) {
    pos = pos - position;
    pos.x /= cellSize;
    pos.y /= cellSize;
    return pos;
}

void Grid::clear_path() {
    for (size_t i = 0; i < MAXROWS*MAXCOLUMNS; i++) {
        if (grid[i] == Selected || grid[i] == Corrected) grid[i] = Walkable;
    }
}

void Grid::clear() {
    for (size_t i = 0; i < MAXROWS*MAXCOLUMNS; i++) {
        grid[i] = Walkable;
    }
}
void Grid::set_bound(SDL_Rect b) {
    bound = b;
    float temp_cell_w = b.w*1.0f / MAXCOLUMNS;
    float temp_cell_h = b.h*1.0f / MAXROWS;
    if (temp_cell_w < temp_cell_h) {
        cellSize = size_t(temp_cell_w);
        position.x = b.x;
        position.y = b.y + b.h/2.0f - temp_cell_w*MAXROWS/2.0f;
    } else {
        cellSize = size_t(temp_cell_h);
        position.x = b.x + b.w/2.0f - temp_cell_h*MAXCOLUMNS/2.0f;
        position.y = b.y;
    }
}
