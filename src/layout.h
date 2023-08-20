#include "widget.h"
#include <vector>
#include <string>
#include <optional>
#include <cassert>
#include <memory>

struct Layout: public Widget {
    Layout(std::string_view type): type(type), widgets(0) {}
    Layout(std::string_view type, size_t nWidgets): type(type), widgets(nWidgets) {}
    Layout(SDL_Rect bound, std::string_view type): Widget(bound), type(type), widgets(0) {}
    Layout(SDL_Rect bound, std::string_view type, size_t nWidgets): Widget(bound), type(type), widgets(nWidgets) {}
    virtual ~Layout() = default;
    virtual void update() = 0;
    inline SDL_Rect get_bound(size_t i) {
        assert(i < widgets.size() && "Overflow");
        return widgets[i]->bound;
    }
    virtual void add(std::shared_ptr<Widget> widget) {
        widgets.push_back(widget);
    }
    inline void draw(SDL_Renderer* renderer) {
        for (auto& widget : widgets) {
            widget->draw(renderer);
        }
    }
    inline void set_bound(SDL_Rect bound) {
        this->bound = bound;
        update();
    }
    std::string type;
    std::vector<std::shared_ptr<Widget>> widgets;
};

struct HorizontalLayout: public Layout {
    HorizontalLayout(): Layout("Horizontal") {}
    HorizontalLayout(std::initializer_list<float> portions): Layout("Horizontal"), portions(std::vector<float>(portions)) {}
    HorizontalLayout(SDL_Rect bound): Layout(bound, "Horizontal") {}
    HorizontalLayout(SDL_Rect bound, std::initializer_list<float> portions): Layout(bound, "Horizontal"), portions(std::vector<float>(portions)) {
        float sum = 0;
        for (auto& p : portions)
            sum += p;
        assert(abs(sum - 1.0f) < 1e-9);
    }
    inline void add(std::shared_ptr<Widget> widget) override {
        assert(!portions || widgets.size() < portions->size());
        widgets.push_back(widget);
        update();
    }
    inline void update() override {
        int x = bound.x;
        for (size_t i = 0; i < widgets.size(); i++) {
            const int compWidth = portions ? bound.w * (*portions)[i] : int(bound.w/widgets.size());
            widgets[i]->set_bound(SDL_Rect {
                .x = x,
                .y = bound.y,
                .w = compWidth,
                .h = bound.h,
            });
            x += compWidth;
        }
    }
    std::optional<std::vector<float>> portions;
};

struct VerticalLayout: public Layout {
    VerticalLayout(): Layout("Vertical") {}
    VerticalLayout(std::initializer_list<float> portions): Layout("Vertical"), portions(std::vector<float>(portions)) {}
    VerticalLayout(SDL_Rect bound): Layout(bound, "Vertical") {}
    VerticalLayout(SDL_Rect bound, std::initializer_list<float> portions): Layout(bound, "Horizontal"), portions(std::vector<float>(portions)) {}
    inline void add(std::shared_ptr<Widget> widget) override {
        assert(!portions || widgets.size() < portions->size());
        widgets.push_back(widget);
        update();
    }
    inline void update() override {
        int y = bound.y;
        for (size_t i = 0; i < widgets.size(); i++) {
            const int compHeight = portions ? bound.h * (*portions)[i] : int(bound.h/widgets.size());
            widgets[i]->set_bound(SDL_Rect {
                .x = bound.x,
                .y = y,
                .w = bound.w,
                .h = compHeight
            });
            y += compHeight;
        }
    }
    std::optional<std::vector<float>> portions;
};

struct GridLayout: public Layout {
    GridLayout(size_t rows, size_t cols): Layout("Grid", rows*cols), nRows(rows), nCols(cols) {}
    GridLayout(SDL_Rect bound, size_t rows, size_t cols): Layout(bound, "Grid", rows*cols), nRows(rows), nCols(cols) {}
    inline void add(std::shared_ptr<Widget> widget) override {
        assert(widgets.size() < nRows*nCols && "Can't add more");
        widgets.push_back(widget);
        float compWidth = bound.w/nCols;
        float compHeight = bound.w/nRows;
        size_t r = (widgets.size()-1)/nCols;
        size_t c = (widgets.size()-1)%nCols;
        widgets[widgets.size()-1]->set_bound(SDL_Rect {
            .x = int(bound.x + compWidth*c),
            .y = int(bound.y + compHeight*r),
            .w = int(compWidth),
            .h = int(compHeight),
        });
    }
    inline void update() override {
        float compWidth = bound.w/nCols;
        float compHeight = bound.w/nRows;
        for (size_t i = 0; i < nRows; i++) {
            for (size_t j = 0; j < nCols; j++) {
                if (i*nCols + j >= widgets.size()) return;
                widgets[i*nCols + j]->set_bound(SDL_Rect {
                    .x = int(bound.x + compWidth*j),
                    .y = int(bound.y + compHeight*i),
                    .w = int(compWidth),
                    .h = int(compHeight),
                });
            }
        }
    }
    size_t nRows;
    size_t nCols;
};
