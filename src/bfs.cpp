#include "Findpath.h"
#include <list>

inline Vec2i directions[] = {
    {1, 1}, {0, 1}, {1, 0}, {1, -1}, {-1, 1}, {-1, 0}, {0, -1}, {-1, -1},
};

class BFSFinder: public PathFinder {
public:
    struct Node {
        Vec2i pos;
        std::shared_ptr<Node> parent;
        Node(Vec2i p, std::shared_ptr<Node> parent): pos{ p }, parent{ parent } {}
    };

    BFSFinder(): PathFinder("BFS") {}
    float find(std::shared_ptr<Grid> grid, Vec2i start, Vec2i end) override {
        std::list<std::shared_ptr<Node>> frontier;
        frontier.push_back(std::make_shared<Node>(start, nullptr));
        while (frontier.size() > 0) {
            auto node = frontier.front();
            frontier.pop_front();
            if (node->pos == end) return mark_path(grid, node);
            gen_children(&frontier, grid, node);
        }
        return -1;
    }

    float mark_path(std::shared_ptr<Grid> grid, std::shared_ptr<Node> cur) {
        float cost = 0;
        while (true) {
            grid->at(cur->pos) = Corrected;
            if (cur->parent) cost += cur->pos.getDistance(cur->parent->pos);
            else return cost;
            cur = cur->parent;
        }
        return -1;
    }

    void gen_children(std::list<std::shared_ptr<Node>>* frontier, std::shared_ptr<Grid> grid, std::shared_ptr<Node> cur) {
        for (const auto& dir : directions) {
            Vec2i child_pos = cur->pos + dir;
            if (grid->is_valid_pos(child_pos) && grid->at(child_pos) == Walkable) {
                frontier->push_back(std::make_shared<Node>(child_pos, cur));
                grid->at(child_pos) = Selected;
            }
        }
    }
};

extern "C" {
    std::shared_ptr<PathFinder> get_object(void) {
        return std::make_shared<BFSFinder>();
    }
}
