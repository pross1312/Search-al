#include "Button.h"
#include "utils.h"

template<typename T>
T* check(T* data);
void check(int data);

Button::Button(const char* msg, TTF_Font* font, SDL_Renderer* renderer, std::function<void()> onClickedEvent):
    text{ msg },
    currentColor{ BUTTON_NORMAL_COLOR },
    onClickedEvent{ onClickedEvent } {
        currentColor = BUTTON_NORMAL_COLOR;
        SDL_Surface* text = check(TTF_RenderText_Solid(font,  msg, SDL_Color{UNHEX(uint8_t, BUTTON_TEXT_COLOR)}));
        textBound.w = text->w;
        textBound.h = text->h;
        setBound(SDL_Rect{ .x = 0, .y = 0, .w = text->w + 2*padding, .h = text->h + 2*padding });
        texture = check(SDL_CreateTextureFromSurface(renderer, text));
        SDL_FreeSurface(text);
}

Button::Button(const char* msg, TTF_Font* font, SDL_Renderer* renderer, int x, int y, std::function<void()> onClickedEvent):
    text{ msg },
    currentColor{ BUTTON_NORMAL_COLOR },
    onClickedEvent{ onClickedEvent } {
        currentColor = BUTTON_NORMAL_COLOR;
        SDL_Surface* text = check(TTF_RenderText_Solid(font,  msg, SDL_Color{UNHEX(uint8_t, BUTTON_TEXT_COLOR)}));
        textBound.w = text->w;
        textBound.h = text->h;
        setBound(SDL_Rect{ .x = x, .y = y, .w = text->w + 2*padding, .h = text->h + 2*padding });

        texture = check(SDL_CreateTextureFromSurface(renderer, text));
        SDL_FreeSurface(text);
}

void Button::draw(SDL_Renderer* renderer) {
    float wRatio = (bound.w - 2.0f*padding)/textBound.w;
    float hRatio = (bound.h - 2.0f*padding)/textBound.h;
    float ratio = wRatio < hRatio ? wRatio : hRatio;
    SDL_Rect dst = {
        .x = int(bound.x + bound.w/2.0f - textBound.w*ratio/2.0f),
        .y = int(bound.y + bound.h/2.0f - textBound.h*ratio/2.0f),
        .w = int(textBound.w*ratio),
        .h = int(textBound.h*ratio),
    };
    check(SDL_SetRenderDrawColor(renderer, UNHEX(uint8_t, currentColor)));
    check(SDL_RenderFillRect(renderer, &bound));
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
void Button::setBound(SDL_Rect b) {
    bound = b;
    padding = (b.w < b.h ? b.w : b.h)/10;
}
