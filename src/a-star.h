#pragma once
#include "Findpath.h"
#include <functional>
#include <vector>
#include <memory>
class AStarFinder: public PathFinder {
public:
    struct Node {
        Vec2i pos;
        float pathCost, heuristic;
        std::shared_ptr<Node> parent;
        inline int cost() const { return pathCost + heuristic; };
        inline bool operator>(const Node& b) const { return this->cost() > b.cost(); }
    };
    using NodePtr = std::shared_ptr<Node>;
    // using Frontier = std::priority_queue<NodePtr, std::vector<NodePtr>, std::function<bool(const NodePtr&, const NodePtr&)>>;
    using Frontier = std::vector<NodePtr>;

    AStarFinder(): PathFinder("A-star") {}

    virtual float find(std::shared_ptr<Grid> g, Vec2i _start, Vec2i _end) override;
    NodePtr checkInFrontier(Vec2i nodePos);
    void addChildToFrontier(NodePtr parent);
    void markFoundPath(NodePtr goalNode);

private:
    Frontier frontier;
    std::shared_ptr<Grid> grid         = nullptr;
    Vec2i start;
    Vec2i goal;
};
