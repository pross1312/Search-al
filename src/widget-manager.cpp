#include "widget-manager.h"
#include <assert.h>

void WidgetManager::update() {
    if (widgets.size() == 0) return;
    switch (layout) {
    case Vertical: {
        float compHeight = bound.h/widgets.size();
        for (size_t i = 0; i < widgets.size(); i++) {
            widgets[i]->setBound(SDL_Rect {
                .x = bound.x,
                .y = int(bound.y + compHeight*i),
                .w = bound.w,
                .h = int(compHeight)
            });
        }
    } break;
    case Horizontal: {
        float compWidth = bound.w/widgets.size();
        for (size_t i = 0; i < widgets.size(); i++) {
            widgets[i]->setBound(SDL_Rect {
                .x = int(bound.x + compWidth*i),
                .y = bound.y,
                .w = int(compWidth),
                .h = bound.h,
            });
        }
    } break;
    case PortionHorizontal: {
        int x = bound.x;
        for (size_t i = 0; i < widgets.size(); i++) {
            float compWidth = bound.w*portions[i];
            widgets[i]->setBound(SDL_Rect {
                .x = x,
                .y = bound.y,
                .w = int(compWidth),
                .h = bound.h,
            });
            x += int(compWidth);
        }
    } break;
    case GridLayout: {
        float compWidth = bound.w/nCols;
        float compHeight = bound.w/nRows;
        for (size_t i = 0; i < nRows; i++) {
            for (size_t j = 0; j < nCols; j++) {
                if (i*nCols + j >= widgets.size()) return;
                widgets[i*nCols + j]->setBound(SDL_Rect {
                    .x = int(bound.x + compWidth*j),
                    .y = int(bound.y + compHeight*i),
                    .w = int(compWidth),
                    .h = int(compHeight),
                });
            }
        }
    } break;
    default:
        assert(false && "Unregconized layout");
    }
}

void WidgetManager::draw(SDL_Renderer* renderer) {
    for (auto& widget : widgets) {
        widget->draw(renderer);
    }
}

void WidgetManager::setBound(SDL_Rect b) {
    bound = b;
    update();
}
