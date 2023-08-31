#include "Findpath.h"
#include <algorithm>
#include <optional>

inline Vec2i directions[] = {
    {1, 1}, {0, 1}, {1, 0}, {1, -1}, {-1, 1}, {-1, 0}, {0, -1}, {-1, -1},
};

struct DijikstraFinder: public PathFinder {
    struct Node {
        float value;
        std::optional<Vec2i> parent;
        Node(): value(1e9f), parent(std::nullopt) {}
        Node(float v, Vec2i p): value(v), parent(p) {}
    };
    DijikstraFinder(): PathFinder("Dijkstra") {};

    virtual inline float find(std::shared_ptr<Grid> _grid, Vec2i start, Vec2i end) override {
        this->grid = _grid;
        this->values.clear();
        this->values.resize(grid->MAXROWS*grid->MAXCOLUMNS, Node());
        this->unexplored.clear();
        node_at(start).value = 0;
        auto cmp = [&](const Vec2i& a, const Vec2i& b) {
            return node_at(a).value > node_at(b).value;
        };
        for (size_t r = 0; r < grid->MAXROWS; r++) {
            for (size_t c = 0; c < grid->MAXCOLUMNS; c++) {
                if (grid->at(r, c) == Walkable) unexplored.push_back(Vec2i(c, r));
            }
        }
        std::sort(unexplored.begin(), unexplored.end(), cmp);
        while (unexplored.size() != 0) {
            Vec2i cur = unexplored.back();
            unexplored.pop_back();
            if (node_at(cur).value == 1e9f || grid->at(cur) != Walkable) continue; // somehow we got to an invalid state then skip it
            grid->at(cur) = Selected;
            if (cur == end) return mark_path(cur);
            update_children(cur);
            std::sort(unexplored.begin(), unexplored.end(), cmp);
        }
        return -1;
    }

    float mark_path(Vec2i pos) {
        float cost = 0;
        while (true) {
            grid->at(pos) = Corrected;
            auto temp = node_at(pos).parent;
            if (temp) cost += pos.getDistance(*temp);
            else return cost;
            pos = *temp;
        }
        return cost;
    }

    void update_children(Vec2i cur) {
        for (auto dir : directions) {
            Vec2i child_pos = cur + dir;
            if (grid->is_valid_pos(child_pos) && grid->at(child_pos) == Walkable) {
                float new_cost = node_at(cur).value + cur.getDistance(child_pos);
                Node& child_node = node_at(child_pos);
                if (child_node.value > new_cost) {
                    child_node.value = new_cost;
                    child_node.parent = cur;
                }
            }
        }
    }

    Node& node_at(Vec2i pos) { assert(grid); return values[pos.y*grid->MAXCOLUMNS + pos.x]; }

    std::shared_ptr<Grid> grid;
    std::vector<Node> values;
    std::vector<Vec2i> unexplored;
};

extern "C" {
    std::shared_ptr<PathFinder> get_object(void) {
        return std::make_shared<DijikstraFinder>();
    }
}
