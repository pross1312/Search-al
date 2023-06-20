#pragma once
#include "Cell.h"
#include "function.h"

class Grid {
public:
    Grid(int rows, int cols, SDL_Renderer* screen);
    ~Grid();

    // get mouse click cell
    Position pointToGridCell(Position pos);
    bool isValidPosition(Position p) { return p.x >= 0 && p.y >= 0 && p.x < MAXCOLUMNS&& p.y < MAXROWS; }

    // clear grid and set to default
    void clear();

    // draw entire grid to screen
    void draw(SDL_Renderer* screen);

    // read and write map file
    void saveToFile(const char* fName);
    void readFromFile(const char* fName);

    void setState(Position pos, State s) { grid[pos.y][pos.x].setState(s); }
    void setCellWalkable(Position p, bool walkable);
    Cell& getCell(Position p) { return grid[p.y][p.x]; }

    const int MAXROWS, MAXCOLUMNS;

private:
    Cell** grid;
};
