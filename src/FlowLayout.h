#pragma once
#include "layout.h"

struct FlowLayout: public Layout {
    enum Type {
        Horizontal,
        Vertical,
    };
    FlowLayout(FlowLayout::Type t): Layout("Horizontal"), type{ t } {}
    FlowLayout(FlowLayout::Type t, SDL_Rect bound): Layout(bound, "Horizontal"), type{ t } {}
    void add(std::shared_ptr<Widget> widget) override {
        widgets.push_back(widget);
        update();
    }
    // insert at the end if index out of bound
    void add(std::shared_ptr<Widget> widget, size_t index) override {
        if (index > widgets.size()) index = widgets.size();
        widgets.insert(widgets.begin() + index, widget);
        update();
    }
    void update() override {
        if (widgets.size() == 0) return;
        if (type == Type::Horizontal) update_horizontal();
        else if (type == Type::Vertical) update_horizontal();
        else assert(false && "Unimplemented");
    }
    void update_vertical() {
        int y = bound.y;
        const int comp_height = int(bound.h/widgets.size());
        for (size_t i = 0; i < widgets.size(); i++) {
            widgets[i]->set_bound(SDL_Rect {
                .x = bound.x,
                .y = y,
                .w = bound.w,
                .h = comp_height,
            });
            y += comp_height;
        }
    }
    void update_horizontal() {
        int x = bound.x;
        const int comp_width = int(bound.w/widgets.size());
        for (size_t i = 0; i < widgets.size(); i++) {
            widgets[i]->set_bound(SDL_Rect {
                .x = x,
                .y = bound.y,
                .w = comp_width,
                .h = bound.h,
            });
            x += comp_width;
        }
    }

    Type type;
};
