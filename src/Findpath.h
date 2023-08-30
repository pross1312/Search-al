#pragma once
#include "Grid.h"
#include <string>
#include <memory>

class PathFinder {
public:
    PathFinder(const char *name): name(name) {}
    virtual ~PathFinder() = default;
    virtual void find(std::shared_ptr<Grid> grid, Vec2i start, Vec2i end) = 0;
    std::string name;
};
