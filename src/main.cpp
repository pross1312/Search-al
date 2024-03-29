#include "Button.h"
#include "text.h"
#include "PortionLayout.h"
#include "FlowLayout.h"
#include "GridLayout.h"
#include "Grid.h"
#include "Findpath.h"
#include <filesystem>
#include <dlfcn.h>

#define FPS 60

using std::make_shared;
using std::shared_ptr;
namespace fs = std::filesystem;

// Vec2i startPos = { 0, 0 };
// Vec2i endPos = { ROWS-5, COLUMNS-5 };
#define GET_OBJECT_FUNCTION_NAME "get_object"
using GetObjectFunction = shared_ptr<PathFinder> (*)(void);
std::vector<void*> plugins;
std::vector<shared_ptr<PathFinder>> finders;

int main(void) {
    const int rows = 20;
    const int columns = 20;
    const char* searchAlgoPath = "search-algo/";
    const char* fontPath       = "JetBrainsMono-Regular.ttf";

    size_t S_WIDTH = 800;
    size_t S_HEIGHT = 700;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;

    check(SDL_Init(SDL_INIT_VIDEO));
    check(TTF_Init());
    window = check(SDL_CreateWindow("PATH",
                   SDL_WINDOWPOS_UNDEFINED,
                   SDL_WINDOWPOS_UNDEFINED,
                   S_WIDTH,
                   S_HEIGHT,
                   SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));
    renderer = check(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

    bool quit                  = false;
    bool on_left_mouse_click   = false;
    bool on_right_mouse_click  = false;
    bool dirty                 = false;

    TTF_Font* font = check(TTF_OpenFont(fontPath, 24));

    auto grid = make_shared<Grid>(rows, columns);
/*
 * ----------------------------------------------*
 *                             |reset | clearpath*
 *                             |-----------------*
 *                             |                 *
 *                             |                 *
 *      grid                   |   search algo   *
 *                             |                 *
 *                             |                 *
 *                             |                 *
 *                             |                 *
 * ----------------------------------------------*
*/

    auto all_layout = make_shared<PortionLayout>(
            PortionLayout::Type::Horizontal,
            SDL_Rect{ .x = 0, .y = 0, .w = int(S_WIDTH), .h = int(S_HEIGHT) },
            std::initializer_list{0.8f, 0.2f});
    auto sub_layout  = make_shared<PortionLayout>(
            PortionLayout::Type::Vertical,
            std::initializer_list{0.1f, 0.8f, 0.1f});
    auto sub_layout1 = make_shared<FlowLayout>(
            FlowLayout::Type::Horizontal);
    auto performance_text = make_shared<Text>("Checking", font, renderer, 0xffffffff, 0, 0);
    all_layout->add(grid);
    all_layout->add(sub_layout);
    sub_layout->add(sub_layout1, 0);
    sub_layout->add(performance_text, 2);

    std::vector<shared_ptr<Button>> buttons {
        make_shared<Button>("Reset", font, renderer, [&]() { grid->clear(); dirty = false; }),
        make_shared<Button>("Clear", font, renderer, [&]() { grid->clear_path(); dirty = false; }),
    };

    sub_layout1->add(buttons[0]);
    sub_layout1->add(buttons[1]);

    // load search algorithm
    for (auto const& entry : fs::directory_iterator(searchAlgoPath)) {
        auto path = entry.path().c_str();
        void* plugin = dlopen(path, RTLD_NOW);
        if (plugin == NULL) {
            fprintf(stderr, "Can't load %s, ERROR: %s\n", path, dlerror());
            continue;
        }
        GetObjectFunction get_object = (GetObjectFunction)dlsym(plugin, GET_OBJECT_FUNCTION_NAME);
        if (get_object == NULL) {
            fprintf(stderr, "Can't find getObject, ERROR: %s\n", dlerror());
            continue;
        }
        size_t index = finders.size();
        plugins.push_back(plugin);
        const auto& finder = get_object();
        finders.push_back(finder);
        buttons.push_back(make_shared<Button>(finder->name.c_str(), font, renderer, [&, renderer, index, performance_text]() {
            if (dirty) return;
            dirty = true;
            float cost = finders[index]->find(grid, Vec2i{0, 0}, Vec2i{(int)grid->MAXCOLUMNS-1, (int)grid->MAXROWS-1});
            if (cost == -1) performance_text->set_text(renderer, "Not found");
            else {
                char text_buffer[16] {};
                sprintf(text_buffer, "%.3f", cost);
                performance_text->set_text(renderer, text_buffer);
            }
        }));
        // close and free later or just leave it
    }

    auto sub_layout2 = make_shared<GridLayout>(buttons.size() - 2 < 10 ? 5 : (buttons.size()-2)/2+1, 2);
    for (size_t i = 2; i < buttons.size(); i++) sub_layout2->add(buttons[i]);

    sub_layout->add(sub_layout2);

    while (!quit) {
        uint32_t start = SDL_GetTicks();
        Vec2i mousePos;
        SDL_GetMouseState(&mousePos.x, &mousePos.y);
        while (SDL_PollEvent(&event) != 0) {
            for (auto& b : buttons) b->update(event, mousePos);
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) quit = true;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    S_WIDTH = event.window.data1;
                    S_HEIGHT = event.window.data2;
                    all_layout->set_bound(SDL_Rect{ .x = 0, .y = 0, .w = int(S_WIDTH), .h = int(S_HEIGHT)});
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    on_right_mouse_click = true;
                    on_left_mouse_click = false;
                }
                else if (event.button.button == SDL_BUTTON_LEFT) {
                    on_left_mouse_click = true;
                    on_right_mouse_click = false;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) on_left_mouse_click = false;
                else if (event.button.button == SDL_BUTTON_RIGHT) on_right_mouse_click = false;
                break;
            }
        }
        if (on_left_mouse_click || on_right_mouse_click) {
            Vec2i cell = grid->point_to_cell(mousePos);
            if (grid->is_valid_pos(cell)) grid->at(cell) = on_left_mouse_click ? Unwalkable : Walkable;
        }
        check(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255));
        check(SDL_RenderClear(renderer));
        all_layout->draw(renderer);
        SDL_RenderPresent(renderer);

        uint32_t interval = SDL_GetTicks() - start;
        if (interval < 1000.0f/FPS) {
            SDL_Delay(1000.0f/FPS - interval);
        }
    }
    return 0;
}
