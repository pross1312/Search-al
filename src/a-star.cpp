#include "a-star.h"
#include <memory>
#include <algorithm>

Vec2i directions[] = {
    {0, 1}, {0, -1},
    {1, 0}, {-1, 0},
    {1, 1}, {-1, -1},
    {1, -1}, {-1, 1},
};


float AStarFinder::find(std::shared_ptr<Grid> _grid, Vec2i _start, Vec2i _end) {
    this->grid   = _grid;
    this->start  = _start;
    this->goal   = _end;
    // this->frontier = new Frontier(std::bind(&AStarFinder::comp, this, std::placeholders::_1, std::placeholders::_2));
    frontier.clear();

    frontier.push_back(std::make_shared<Node>(Node{
            .pos = start,
            .pathCost = 0.0f,
            .heuristic = _start.getDistance(_end),
            .parent = nullptr,
        }));

    while (!frontier.empty()) {
        NodePtr top = frontier.back();
        grid->at(top->pos) = Selected;
        frontier.pop_back();
        if (top->pos == goal) {
            markFoundPath(top);
            return top->pathCost;
        }
        addChildToFrontier(top);
        std::sort(frontier.begin(), frontier.end(), [](auto a, auto b) { return *a > *b; });
    }
    return -1;
}

void AStarFinder::markFoundPath(NodePtr goalNode) {
    while (goalNode != nullptr) {
        grid->at(goalNode->pos) = Corrected;
        goalNode = goalNode->parent;
    }
}

AStarFinder::NodePtr AStarFinder::checkInFrontier(Vec2i nodePos) {
    for (const auto& ptr : frontier) {
        if (ptr->pos == nodePos) return ptr;
    }
    return nullptr;
}

void AStarFinder::addChildToFrontier(NodePtr parent) {
    Vec2i center = parent->pos;
    for (auto dir : directions) {
        Vec2i childPos = center + dir;
        if (grid->is_valid_pos(childPos) && grid->at(childPos) == Walkable) { // explored can be checked with selected
            NodePtr childNode = std::make_shared<Node>(Node{
                .pos = childPos,
                .pathCost = parent->pathCost + parent->pos.getDistance(childPos),
                .heuristic = childPos.getDistance(goal),
                .parent = parent
            });
            NodePtr checkNode = checkInFrontier(childPos);
            if (checkNode == nullptr) {
                frontier.push_back(childNode);
            } else if (checkNode->cost() > childNode->cost()) {
                *checkNode = *childNode;
            }
        }
    }
}

extern "C" {
    std::shared_ptr<PathFinder> get_object(void) {
        return std::make_shared<AStarFinder>();
    }
}
