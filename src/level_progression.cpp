#include "level_progression.hpp"
#include "application_state.hpp"
#include "constants.hpp"
#include "key_macros.hpp"
#include "sokoban.hpp"

#include <fstream>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <ncurses.h>

#include <filesystem>

extern bool RUNNING;

const int LEVELS_PER_ROW = 5;
const int SPACE_BETWEEN_LEVELS = 5;

LevelProgression::LevelProgression(ApplicationState& _state) : State(_state){
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
        CASE_UP_KEYS
            application_state.level_id = std::max(0, application_state.level_id - LEVELS_PER_ROW);
            break;
        CASE_DOWN_KEYS
            application_state.level_id = std::min(
                static_cast<int>(levels.size()) - 1,
                application_state.level_id + LEVELS_PER_ROW
            );
            break;
        CASE_LEFT_KEYS
            application_state.level_id = std::max(0, application_state.level_id - 1);
            break;
        CASE_RIGHT_KEYS
            application_state.level_id = std::min(
                static_cast<int>(levels.size()) - 1,
                application_state.level_id + 1
            );
            break;
        CASE_Q_KEYS
            return menu;
        CASE_SELECT_KEYS {
            if (application_state.level_id > application_state.level_data.size()) break;

            ///////////////////////////////////////////////////////////////////
            /// @TODO: this is bad placement, and should be part of Sokoboan,
            /// but we aren't going to make any updates until we get to
            /// improving the way files are read. Ideally, we should just pass
            /// a file path to sokoban, and that's it.
            std::ifstream level_file(levels[application_state.level_id]);
            if (!level_file.good()) {
                std::cerr << "Could not open file: " << levels[application_state.level_id] << std::endl;
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

            game->start_time = clock();
            game->moves = 0;

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

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    int y = max_y / 6;
    move(y, (max_x - 17) / 2); // 17 is length of "Level Progression"
    printw("Level Progression");

    const auto unlocked_regular = COLOR_PAIR(COLOR_UNLOCKED);
    const auto unlocked_highlighted = COLOR_PAIR(COLOR_UNLOCKED_HIGHLIGHTED);
    const auto locked_regular = COLOR_PAIR(COLOR_LOCKED);
    const auto locked_highlighted = COLOR_PAIR(COLOR_LOCKED_HIGHLIGHTED);

    const int min_x = (max_x - (LEVELS_PER_ROW - 1) * SPACE_BETWEEN_LEVELS) / 2;
    int x = min_x;
    y += 2;

    int color;
    for (std::size_t i = 0; i < levels.size(); ++i) {
        if (i % LEVELS_PER_ROW == 0) {
            x = min_x;
            ++y;
        } else {
            x += SPACE_BETWEEN_LEVELS;
        }

        const bool level_unlocked = i <= application_state.level_data.size();
        if (i == application_state.level_id) {
           color = level_unlocked ? unlocked_highlighted : locked_highlighted;
        } else {
            color = level_unlocked ? unlocked_regular : locked_regular;
        }

        attron(color);
        mvprintw(y, x, "%c%c", levels[i][7], levels[i][8]); // levels/XX.txt, xx in pos 7 and 8
        attroff(color);
    }
}

