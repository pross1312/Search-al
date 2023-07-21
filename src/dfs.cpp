#include "Findpath.h"

inline Vec2i directions[] = {
    {1, 1}, {0, 1}, {1, 0}, {1, -1}, {-1, 1}, {-1, 0}, {0, -1}, {-1, -1},
};

class DFSFinder: public PathFinder {
public:
    DFSFinder() = default;
    virtual inline void find(std::shared_ptr<Grid> g, Vec2i _start, Vec2i _end) override {
        if (_start == _end) {
            g->at(_start) = Corrected;
            found = true;
            return;
        }
        g->at(_start) = Selected;
        for (auto& dir : directions) {
            if (g->isValidPosition(_start + dir) && g->at(_start + dir) == Walkable) {
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
    std::shared_ptr<PathFinder> getObject(void) {
        return std::make_shared<DFSFinder>();
    }
}