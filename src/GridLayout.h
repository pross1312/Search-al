#pragma once
#include "layout.h"
struct GridLayout: public Layout {
    GridLayout(size_t rows, size_t cols): Layout("Grid"), nRows(rows), nCols(cols) {
        widgets.resize(rows*cols);
    }
    GridLayout(SDL_Rect bound, size_t rows, size_t cols): Layout(bound, "Grid"), nRows(rows), nCols(cols) {
        widgets.resize(rows*cols);
    }
    void add(std::shared_ptr<Widget> widget) override {
        size_t i = 0;
        while (widgets[i]) i++;
        widgets[i]             = widget;
        const float compWidth  = bound.w/nCols;
        const float compHeight = bound.w/nRows;
        const size_t r         = (i-1)/nCols;
        const size_t c         = (i-1)%nCols;
        widgets[i]->set_bound(SDL_Rect {
            .x = int(bound.x + compWidth*c),
            .y = int(bound.y + compHeight*r),
            .w = int(compWidth),
            .h = int(compHeight),
        });
    }
    void add(std::shared_ptr<Widget> widget, size_t index) override {
        widgets[index] = widget;
        update();
    }
    void add(std::shared_ptr<Widget> widget, size_t r, size_t c) {
        assert(widgets.size() < nRows*nCols && r < nRows && c < nCols);
        widgets[r*nCols + c] = widget;
        update();
    }
    void update() override {
        float compWidth = bound.w/nCols;
        float compHeight = bound.w/nRows;
        for (size_t i = 0; i < nRows; i++) {
            for (size_t j = 0; j < nCols; j++) {
                if (i*nCols + j >= widgets.size()) return;
                if (widgets[i*nCols + j])
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
