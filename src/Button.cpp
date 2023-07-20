#include "Button.h"
#include "utils.h"

template<typename T>
T* check(T* data);
void check(int data);

Button::Button(const char* msg, TTF_Font* font, SDL_Renderer* renderer, int x, int y, std::function<void()> onClickedEvent):
    text{ msg },
    currentColor{ BUTTON_NORMAL_COLOR },
    onClickedEvent{ onClickedEvent } {
    currentColor = BUTTON_NORMAL_COLOR;
    SDL_Surface* text = check(TTF_RenderText_Solid(font,  msg, SDL_Color{UNHEX(uint8_t, BUTTON_TEXT_COLOR)}));

    bound.x = x;
    bound.y = y;
    bound.w = text->w + BUTTON_PADDING*2;
    bound.h = text->h + BUTTON_PADDING*2;

    texture = check(SDL_CreateTextureFromSurface(renderer, text));
    SDL_FreeSurface(text);
}

void Button::draw(SDL_Renderer* renderer) {
    check(SDL_SetRenderDrawColor(renderer, UNHEX(uint8_t, currentColor)));
    check(SDL_RenderFillRect(renderer, &bound));
    SDL_Rect dst {
        .x = bound.x + BUTTON_PADDING,
        .y = bound.y + BUTTON_PADDING,
        .w = bound.w - BUTTON_PADDING*2,
        .h = bound.h - BUTTON_PADDING*2,
    };
    check(SDL_RenderCopy(renderer, texture, nullptr, &dst));
}

bool Button::isInBound(Vec2i pos) {
    if (pos.y >= bound.y && pos.y < bound.y + bound.h &&
        pos.x >= bound.x && pos.x < bound.x + bound.w)
        return true;
    return false;
}

void Button::update(SDL_Event& event, Vec2i mousePos) {
    if (isInBound(mousePos)) {
        currentColor = BUTTON_ON_HOVER_COLOR;
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            onClickedEvent();
            currentColor = BUTTON_CLICKED_COLOR;
        }
    } else {
        currentColor = BUTTON_NORMAL_COLOR;
    }
}
