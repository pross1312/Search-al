#pragma once
#include "widget.h"
#include <assert.h>
#include <vector>

enum Layout {
    Vertical = 0,
    Horizontal,
    GridLayout,
    PortionHorizontal,
};

class WidgetManager: public Widget {
public:
    WidgetManager(Layout type): layout(type) {}
    WidgetManager(Layout type, std::vector<float> portions): layout(type), portions(portions) {
        assert(type == PortionHorizontal);
        float sum = 0;
        for (auto f : portions) sum += f;
        assert(std::abs(sum - 1) < 1e-10);
    }
    WidgetManager(SDL_Rect b, Layout type, std::vector<float> portions): WidgetManager(b, type) {
        this->portions = portions;
        assert(type == PortionHorizontal);
        float sum = 0;
        for (auto f : portions) sum += f;
        assert(std::abs(sum - 1) < 1e-10);
    }
    WidgetManager(Layout type, size_t rows, size_t cols): layout(type), nRows(rows), nCols(cols) {
        assert(type == GridLayout);
    }
    WidgetManager(SDL_Rect b, Layout type): Widget(b), layout(type) {}
    // ~WidgetManager() { for (auto& widget : widgets) delete widget; }
    void draw(SDL_Renderer* renderer) override;
    void setBound(SDL_Rect b);
    inline void add(Widget* widget) {
        widgets.push_back(widget);
        update();
    }
    void update();
private:
    std::vector<Widget*> widgets;
    Layout layout;
    size_t nRows = 0, nCols = 0; // for Grid layout only
    std::vector<float> portions; // for portion layout only
};
