#pragma once
#include "Grid.h"
#include <list>

class PathFinder {
public:
    virtual void find(Grid* grid, Vec2i start, Vec2i end) = 0;
};
