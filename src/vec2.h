#pragma once
#include <cmath>

class Vec2i {
public:
    int x {0}, y {0};
    Vec2i() = default;
    Vec2i(int _x, int _y) : x {_x}, y {_y} {}

    inline float getDistance(const Vec2i& a) {
        Vec2i tmp = *this - a;
        float distance = 100.0f * sqrt(tmp.x*tmp.x + tmp.y*tmp.y);
        return distance;
    }
    inline Vec2i operator+(const Vec2i& a) { return {x + a.x, y + a.y}; }
    inline Vec2i operator-(const Vec2i& a) { return {x - a.x, y - a.y}; }
    inline bool operator==(const Vec2i& a) { return x == a.x && y == a.y; }
    inline bool operator!=(const Vec2i& a) { return !(*this == a); }
};
