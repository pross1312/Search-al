#pragma once
#include "Grid.h"
#include <memory>
#include <list>

class PathFinder {
public:
    virtual ~PathFinder() = default;
    virtual void find(std::shared_ptr<Grid> grid, Vec2i start, Vec2i end) = 0;
};
