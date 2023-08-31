#pragma once
#include "widget.h"
#include <vector>
#include <string>
#include <cassert>
#include <memory>

struct Layout: public Widget {
    Layout(std::string_view type): type(type), widgets(0) {}
    Layout(SDL_Rect bound, std::string_view type): Widget(bound), type(type), widgets(0) {}
    virtual ~Layout() = default;
    virtual void update() = 0;
    SDL_Rect get_bound(size_t i) {
        assert(i < widgets.size() && widgets[i] && "Overflow");
        return widgets[i]->bound;
    }
    virtual void add(std::shared_ptr<Widget> widget) {
        widgets.push_back(widget);
    }
    virtual void add(std::shared_ptr<Widget> widget, size_t index) = 0;
    // virtual std::shared_ptr<Widget> get(std::shared_ptr<Widget> widget, size_t index) = 0;
    // virtual void del(std::shared_ptr<Widget> widget, size_t index) = 0;
    void draw(SDL_Renderer* renderer) {
        for (auto& widget : widgets) {
            if (widget) widget->draw(renderer);
        }
    }
    void set_bound(SDL_Rect bound) {
        this->bound = bound;
        update();
    }
    std::string type;
    std::vector<std::shared_ptr<Widget>> widgets;
};
