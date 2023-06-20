#ifndef FUNCTION
#define FUNCTION

#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL2/SDL.h>

#include <SDL2/SDL_image.h>

#define REDKEY 255
#define BLUEKEY 255
#define GREENKEY 255

class Position {
public:
    int x {0}, y {0};
    Position() = default;
    Position(int _x, int _y) : x {_x}, y {_y} {}

    int getDistance(const Position& a);

    Position operator+(const Position& a) { return {x + a.x, y + a.y}; }
    Position operator-(const Position& a) { return {x - a.x, y - a.y}; }
    bool operator==(const Position& a) { return x == a.x && y == a.y; }
    bool operator!=(const Position& a) { return !(*this == a); }
};



class Object {
public:
    Object() = default;
    Object(int x, int y) { rectDst.x = x; rectDst.y = y; }
    virtual ~Object() { SDL_DestroyTexture(texture); }

    void setPosition(int x, int y) { rectDst.x = x; rectDst.y = y; }
    // SDL_Rect *getRect() { return &rectDst; }

    virtual int getWidth() const { return rectDst.w; }
    virtual int getHeigth() const { return rectDst.h; }
    virtual bool loadImage(const char* path, SDL_Renderer* screen);
    virtual void draw(SDL_Renderer* dst, SDL_Rect* clip);

protected:
    SDL_Rect rectDst{ 0, 0, 0, 0 };
    SDL_Texture* texture = nullptr;
};

inline void Error(const char* msg) {
    printf("MSG: %s %s\n", msg, SDL_GetError());
}

#endif
