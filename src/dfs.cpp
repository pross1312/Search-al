#include "Findpath.h"

inline Vec2i directions[] = {
    {1, 1}, {0, 1}, {1, 0}, {1, -1}, {-1, 1}, {-1, 0}, {0, -1}, {-1, -1},
};

class DFSFinder: public PathFinder {
public:
    DFSFinder(): PathFinder("DFS") {}
    virtual inline void find(std::shared_ptr<Grid> g, Vec2i _start, Vec2i _end) override {
        found = false;
        trueFind(g, _start, _end);
    }
    inline void trueFind(std::shared_ptr<Grid> g, Vec2i _start, Vec2i _end) {
        if (_start == _end) {
            g->at(_start) = Corrected;
            found = true;
            return;
        }
        g->at(_start) = Selected;
        for (auto& dir : directions) {
            if (g->is_valid_pos(_start + dir) && g->at(_start + dir) == Walkable) {
                find(g, _start+dir, _end);
                if (found) {
                    g->at(_start) = Corrected;
                    return;
                }
            }
        }
    }
    bool found = false;
};

extern "C" {
    std::shared_ptr<PathFinder> get_object(void) {
        return std::make_shared<DFSFinder>();
    }
}
