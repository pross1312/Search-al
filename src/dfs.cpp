#include "Findpath.h"

inline Vec2i directions[] = {
    {1, 1}, {0, 1}, {1, 0}, {1, -1}, {-1, 1}, {-1, 0}, {0, -1}, {-1, -1},
};

class DFSFinder: public PathFinder {
public:
    DFSFinder(): PathFinder("DFS") {}
    virtual inline float find(std::shared_ptr<Grid> g, Vec2i _start, Vec2i _end) override {
        cost = 0;
        found = false;
        trueFind(g, _start, _end);
        if (found) return cost;
        return -1;
    }
    inline void trueFind(std::shared_ptr<Grid> g, Vec2i cur, Vec2i _end) {
        if (cur == _end) {
            g->at(cur) = Corrected;
            found = true;
            return;
        }
        g->at(cur) = Selected;
        for (auto& dir : directions) {
            Vec2i child_pos = cur + dir;
            if (g->is_valid_pos(child_pos) && g->at(child_pos) == Walkable) {
                cost += cur.getDistance(child_pos);
                trueFind(g, child_pos, _end);
                if (found) {
                    g->at(cur) = Corrected;
                    return;
                }
                cost -= cur.getDistance(child_pos);
            }
        }
    }
    float cost;
    bool found = false;
};

extern "C" {
    std::shared_ptr<PathFinder> get_object(void) {
        return std::make_shared<DFSFinder>();
    }
}
