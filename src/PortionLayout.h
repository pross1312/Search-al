#pragma once
#include "layout.h"

struct PortionLayout: public Layout {
    enum Type {
        Horizontal,
        Vertical,
    };
    PortionLayout(PortionLayout::Type t, std::initializer_list<float> portions): Layout("Horizontal"), type(t), portions(portions) {
        float sum = 0;
        for (auto& p : portions)
            sum += p;
        assert(abs(sum - 1.0f) < 1e-9);
        widgets.resize(portions.size());
    }
    PortionLayout(PortionLayout::Type t, SDL_Rect bound, std::initializer_list<float> portions): Layout(bound, "Horizontal"), type(t), portions(portions) {
        float sum = 0;
        for (auto& p : portions)
            sum += p;
        assert(abs(sum - 1.0f) < 1e-9);
        widgets.resize(portions.size());
    }
    void add(std::shared_ptr<Widget> widget) override {
        assert(count < widgets.size());
        count++;
        for (auto& p : widgets) {
            if (!p) {
                p = widget;
                break;
            }
        }
        update();
    }
    void add(std::shared_ptr<Widget> widget, size_t index) override {
        assert(index < widgets.size());
        if (!widgets[index]) count++;
        widgets[index] = widget;
        update();
    }
    void update() override {
        if (count == 0) return;
        if (type == PortionLayout::Type::Horizontal) update_horizontal();
        else if (type == PortionLayout::Type::Vertical) update_vertical();
        else assert(false && "not implemented");
    }
    void update_horizontal() {
        int x = bound.x;
        size_t i = 0;
        for (float portion : portions) {
            const int comp_width = bound.w * portion;
            if (widgets[i]) widgets[i]->set_bound(SDL_Rect {
                .x = x,
                .y = bound.y,
                .w = comp_width,
                .h = bound.h,
            });
            x += comp_width;
            i++;
        }
    }
    void update_vertical() {
        int y = bound.y;
        size_t i = 0;
        for (float portion : portions) {
            const int comp_height = bound.h * portion;
            if (widgets[i]) widgets[i]->set_bound(SDL_Rect {
                .x = bound.x,
                .y = y,
                .w = bound.w,
                .h = comp_height
            });
            y += comp_height;
            i++;
        }
    }

    Type type;
    size_t count = 0;
    std::vector<float> portions; // NOTE: Don't use initializer_list it fk wrong....(can only use to initialize vector)
};
