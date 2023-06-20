#pragma once
#include <list>
#include "Grid.h"

class Findpath {
public:
    Findpath(Grid* _grid, Position _start, Position _end) : grid {_grid}, start {_start}, end {_end} {}
    
    // caculate value for all adjacent cell of pos and add them to list
    void addAdjacentPosibleCell(Position pos);

    // draw corrected path
    void drawCorrectedPath();

    // find shortest path from start to end
    bool find();


    // caculate distance for pos
    int calculateValue(Position current, Position pos);

private:
    bool comp(const Position& a, const  Position& b);
    std::list<Position> posibleMoves;
    Grid* grid;
    Position start, current, end;
};