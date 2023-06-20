#pragma once
#include "function.h"


enum State {
    Unselected = 0,
    Selected = 1,
    Corrected = 2,
};

class Cell {
public:
    Cell() = default;
    ~Cell() = default;
    
    // load image to render
    bool initGraphic(SDL_Renderer *screen);

    // draw cell at a position
    void draw(Position position, SDL_Renderer *screen);

    void setState(State s) { state = s; }
    void setWalkable(bool _walkable) { walkable = _walkable; }
    void setVal(int v) { val = v; }
    void setTrace(Position p) { trace = p; }

    Position getTrace() { return trace; }
    int getVal() { return val; }
    bool getWalkable() { return walkable; }
    State getState() { return state; }

    inline static const int SIZE = 32;
private:
    int val = INT32_MAX;
    bool walkable = true;    
    State state = Unselected;
    Position trace;
    Object obj[3];
};
