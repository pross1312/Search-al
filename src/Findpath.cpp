#include <algorithm>
#include <functional>
#include "Findpath.h"

Position direction[4]{
    {0, 1},
    {1, 0},
    {1, 1},
    {1, -1},
};
bool Findpath::comp(const Position& a, const Position& b) {
    return grid->getCell(a).getVal() < grid->getCell(b).getVal();
}

bool Findpath::find() {
    posibleMoves.push_back(start);
    grid->getCell(start).setState(Selected);
    grid->getCell(start).setVal(0);
    grid->getCell(start).setTrace({ -1, -1 });
    while (posibleMoves.size() != 0) {
        auto min = posibleMoves.begin();
        for (auto it = posibleMoves.begin(); it != posibleMoves.end(); it++) {
            if (grid->getCell(*min).getVal() > grid->getCell(*it).getVal()) min = it;
        }
        if (*min == end) {
            posibleMoves.clear();
            return true;
        }
        addAdjacentPosibleCell(*min);
        grid->getCell(*min).setState(Selected);
        posibleMoves.erase(min);
    }
    return false;
}

int Findpath::calculateValue(Position current, Position pos) {
    int a = pos.getDistance(current);
    int b = grid->getCell(current).getVal() - current.getDistance(end);
    int c = pos.getDistance(end);
    return a + b + c;
}


void Findpath::addAdjacentPosibleCell(Position pos) {
    for (auto& i : direction) {
        Position tmp = pos;
        tmp = tmp + i;
        if (grid->isValidPosition(tmp)) {
            Cell& c = grid->getCell(tmp);
            if (tmp == end || (c.getState() == Unselected && c.getWalkable() == true)) {
                int val = calculateValue(pos, tmp);
                if (c.getVal() == INT32_MAX) {
                    posibleMoves.push_back(tmp);
                }
                if (val < c.getVal()) {
                    c.setVal(val);
                    c.setTrace(pos);
                }
            }
        }
        tmp = pos;
        tmp = tmp - i;
        if (grid->isValidPosition(tmp)) {
            Cell& c = grid->getCell(tmp);
            if (tmp == end || (c.getState() == Unselected && c.getWalkable() == true)) {
                int val = calculateValue(pos, tmp);
                if (c.getVal() == INT32_MAX) {
                    posibleMoves.push_back(tmp);
                }
                if (val < c.getVal()) {
                    c.setVal(val);
                    c.setTrace(pos);
                }
            }
        }
    }
}

void Findpath::drawCorrectedPath() {
    Position tmp = end;
    while (tmp != start) {
        Cell& c = grid->getCell(tmp);
        c.setState(Corrected);
        tmp = c.getTrace();
    }
    grid->getCell(start).setState(Corrected);
}
