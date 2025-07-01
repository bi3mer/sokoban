#include "level_progression.hpp"
#include "constants.hpp"
#include "sokoban.hpp"

#include <fstream>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <ncurses.h>

#include <filesystem>

extern bool RUNNING;

LevelProgression::LevelProgression() {
    _load_progression();
}

void LevelProgression::_load_progression() {
    for (const auto& entry : std::filesystem::directory_iterator(LEVELS_DIR)) {
        levels.push_back(entry.path());
    }

    std::sort(levels.begin(), levels.end());
}

State* LevelProgression::update() {
    switch(getch()) {
        case 'Q':
        case 'q':
        case KEY_ESC:
            return menu;
        case 'W':
        case 'w':
        case 'K':
        case 'k':
        case KEY_UP:
            // @TODO: should it wrap down?
            selected_index = (selected_index - 1) % levels.size();
            break;
        case 'S':
        case 's':
        case 'J':
        case 'j':
        case KEY_DOWN:
            // @TODO: macro for key up, key down, etc.?
            // @TODO: should it not wrap up?
            selected_index = (selected_index + 1) % levels.size();
            break;
        case '\n':
        case ' ': {
            ///////////////////////////////////////////////////////////////////
            /// @TODO: this is bad placement, and should be part of Sokoboan,
            /// but we aren't going to make any updates until we get to
            /// improving the way files are read. Ideally, we should just pass
            /// a file path to sokoban, and that's it.
            std::ifstream level_file(levels[selected_index]);
            if (!level_file.good()) {
                std::cerr << "Could not open file: " << levels[selected_index] << std::endl;
                RUNNING = false;
            }

            std::vector<std::string> level;
            std::string line;
            while(getline(level_file, line)) {
                level.push_back(line);
            }

            level_file.close();

            const std::size_t num_columns = level[0].size();
            for(std::size_t i = 1; i < level.size(); ++i) {
                assert(level[i].size() == num_columns);
            }

            sokoban_free(game->state);
            sokoban_init_from_level(game->state, level);

            return game;
            ///////////////////////////////////////////////////////////////////
        }
        default:
            break;
    }

    return this;
}

void LevelProgression::render() const {
    clear();
    move(0,0);
    printw("Level Progression");

    const auto highlighted = COLOR_PAIR(COLOR_HIGHLIGHTED);
    const auto regular = COLOR_PAIR(COLOR_REGULAR);

    int y = 2;
    for (std::size_t i = 0; i < levels.size(); ++i, ++y) {
        move(y, 0);

        const int color = i == selected_index ? highlighted : regular;

        attron(color);
        printw(levels[i].c_str());
        attroff(color);
    }
}

