#include <fstream>
#include "Grid.h"

Grid::Grid(int rows, int cols, SDL_Renderer* screen) : MAXROWS{ rows }, MAXCOLUMNS{ cols } {
    grid = new Cell * [MAXROWS];
    for (int i = 0; i < MAXROWS; i++) {
        grid[i] = new Cell[MAXCOLUMNS];
        for (int j = 0; j < MAXCOLUMNS; j++)
            if (grid[i][j].initGraphic(screen) == false)
                exit(1);
    }
}

Grid::~Grid() {
    for (int i = 0; i < MAXROWS; i++)
        delete[] grid[i];
    delete[] grid;
}

void Grid::saveToFile(const char* fName) {
    std::ofstream fout(fName, std::ios::out);
    if (!fout)
        throw "Can't open file to save";
    for (int i = 0; i < MAXROWS; i++)
        for (int j = 0; j < MAXCOLUMNS; j++)
            fout << grid[i][j].getWalkable() << " ";
    fout.close();
}

void Grid::readFromFile(const char* fName) {
    std::ifstream fin(fName, std::ios::in);
    if (!fin)
        throw "Can't open file to read";
    for (int i = 0; i < MAXROWS; i++)
        for (int j = 0; j < MAXROWS; j++) {
            bool w = false;
            fin >> w;
            grid[i][j].setWalkable(w);
        }
    fin.close();
}

void Grid::draw(SDL_Renderer* screen) {
    for (int i = 0; i < MAXROWS; i++)
        for (int j = 0; j < MAXCOLUMNS; j++)
            grid[i][j].draw({ j * Cell::SIZE, i * Cell::SIZE }, screen);
}

Position Grid::pointToGridCell(Position pos) {
    SDL_GetMouseState(&pos.x, &pos.y);
    pos.x /= Cell::SIZE;
    pos.y /= Cell::SIZE;
    return pos;
}

void Grid::clear() {
    for (int i = 0; i < MAXROWS; i++)
        for (int j = 0; j < MAXCOLUMNS; j++) {
            grid[i][j].setState(Unselected);
            grid[i][j].setWalkable(true);
            grid[i][j].setVal(INT32_MAX);
        }
}

void Grid::setCellWalkable(Position p, bool walkable) {
    if (!isValidPosition(p))
        Error("Invalid position");
    grid[p.y][p.x].setWalkable(walkable);
}