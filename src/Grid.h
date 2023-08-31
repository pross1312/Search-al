#pragma once
#include "widget.h"
#include "vec2.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <cassert>


#define MAX_CELL_SIZE 32

enum State {
    Walkable = 0,
    Unwalkable,
    Selected,
    Corrected,
    StateCount,
};


struct Grid: public Widget {
    inline static uint32_t STATE_COLOR[StateCount] {
        [Walkable] = 0xaaaaaaff,
        [Unwalkable] = 0x7f0000ff,
        [Selected] = 0x22b14cff,
        [Corrected] = 0x00a2e8ff,
    };
    Grid(size_t rows, size_t cols);
    ~Grid() { delete[] grid; };

    Vec2i point_to_cell(Vec2i pos);
    bool is_valid_pos(Vec2i p) { return p.x >= 0 && p.y >= 0 && (size_t)p.x < MAXCOLUMNS && (size_t)p.y < MAXROWS; }
    void clear();
    void clear_path();
    void draw(SDL_Renderer* renderer) override;
    void save(const char* fName);
    void load(const char* fName);

    void set_bound(SDL_Rect b) override;

    inline State& at(Vec2i p)            { assert(is_valid_pos(p)); return grid[p.y * MAXCOLUMNS + p.x]; }
    inline State& at(size_t r, size_t c) { assert(is_valid_pos(Vec2i(c, r))); return grid[r * MAXCOLUMNS + c]; }
    inline size_t pxWidth() const        { return MAXCOLUMNS * cellSize; }
    inline size_t pxHeight() const       { return MAXROWS * cellSize; }

    static inline size_t PADDING         = 3;
    static inline uint32_t OUTLINE_COLOR = 0x4f4f87ff;

    const size_t MAXROWS, MAXCOLUMNS;
    size_t cellSize = MAX_CELL_SIZE;
    Vec2i position;
    State* grid;
};
