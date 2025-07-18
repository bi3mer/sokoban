#include "level_progression.hpp"
#include "application_state.hpp"
#include "constants.hpp"
#include "key_macros.hpp"
#include "sokoban.hpp"
#include "log.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <format>
#include <algorithm>
#include <cstddef>
#include <ncurses.h>
#include <stdio.h>

#include <filesystem>

const int LEVELS_PER_ROW = 5;
const int RENDER_ROWS = 6;
const int SPACE_BETWEEN_LEVELS = 5;

void LevelProgression::_load_progression() {
    for (const auto& entry : std::filesystem::directory_iterator(LEVELS_DIR)) {
        levels.push_back(entry.path());
    }

    std::sort(levels.begin(), levels.end());
}

void LevelProgression::on_enter() {
    Log::info("Level progression :: on_enter");
}

void LevelProgression::on_exit() {
    Log::info("Level progression :: on_exit");
}

State* LevelProgression::update() {
    switch(getch()) {
        CASE_UP_KEYS
            app_state->selected_index = std::max(
                0,
                app_state->selected_index - LEVELS_PER_ROW
            );
            break;
        CASE_DOWN_KEYS
            app_state->selected_index = std::min(
                static_cast<int>(levels.size()) - 1,
                app_state->selected_index + LEVELS_PER_ROW
            );
            break;
        CASE_LEFT_KEYS
            app_state->selected_index = std::max(
                0,
                app_state->selected_index - 1
            );
            break;
        CASE_RIGHT_KEYS
            app_state->selected_index = std::min(
                static_cast<int>(levels.size()) - 1,
                app_state->selected_index + 1
            );
            break;
        CASE_Q_KEYS
            Log::info("level progression :: goto menu");
            return menu;
        CASE_SELECT_KEYS {
            if (app_state->selected_index > app_state->level_data.size()) {
                Log::info(
                    std::format(
                        "level progression :: Locked level selection: {} > {}",
                        app_state->selected_index,
                        app_state->level_data.size()
                    ).c_str()
                );

                break;
            }

            if (sokoban_init_from_level(
                app_state->game_state,
                levels[static_cast<std::size_t>(app_state->selected_index)].c_str())
            ) {
                app_state->running = false;
                return this;
            }

            Log::info("level progression :: goto game");
            return game;
        }
        default:
            break;
    }

    const int levels_per_window = LEVELS_PER_ROW * RENDER_ROWS;
    render_start_row = static_cast<int>(std::floor(app_state->selected_index / (float)levels_per_window)) * RENDER_ROWS;

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

    // render level info
    y += 2;

    if (app_state->selected_index == app_state->level_data.size()) {
        move(y, (max_x - 14) / 2);
        printw("Level unlocked");
    } else if (app_state->selected_index > app_state->level_data.size()) {
        move(y, (max_x - 18) / 2);
        printw("Level not unlocked");
    } else {
        move(y, (max_x - 36) / 2); // 17 is length of "Level Progression"
        const LevelData& ld = app_state->level_data[static_cast<std::size_t>(app_state->selected_index)];
        printw("moves: %i, seconds to beat: %.2f", ld.moves, ld.seconds_played);
    }

    // render fewer moves for level by user if relevant
    char buffer[50];
    if (app_state->game_over_move_message != nullptr) {
        y += 2;

        const LevelData& ld = app_state->level_data[static_cast<std::size_t>(app_state->selected_index)];
        snprintf(buffer, 50, app_state->game_over_move_message, app_state->moves, ld.moves);

        move(y, (max_x - static_cast<int>(strlen(buffer))) / 2);
        printw(buffer);

        app_state->game_over_move_message = nullptr;
    }

    // render less time played for level by user if relevant
    if (app_state->game_over_time_message != nullptr) {
        y += 2;

        const LevelData& ld = app_state->level_data[static_cast<std::size_t>(app_state->selected_index)];
        snprintf(buffer, 50, app_state->game_over_time_message, app_state->seconds_played_message, ld.seconds_played);

        move(y, (max_x - static_cast<int>(strlen(buffer))) / 2);
        printw(buffer);

        app_state->game_over_time_message = nullptr;
    }

    // render levels
    const int min_x = (max_x - (LEVELS_PER_ROW - 1) * SPACE_BETWEEN_LEVELS) / 2;
    int x = min_x;
    y += 2;
    int color;

    int i = LEVELS_PER_ROW * render_start_row;
    const int levels_to_render = std::min(
        i + LEVELS_PER_ROW * RENDER_ROWS,
        static_cast<int>(levels.size())
    );

    for (; i < levels_to_render; ++i) {
        if (i % LEVELS_PER_ROW == 0) {
            x = min_x;
            ++y;
        } else {
            x += SPACE_BETWEEN_LEVELS;
        }

        const bool level_unlocked = i <= app_state->level_data.size();
        if (i == app_state->selected_index) {
           color = level_unlocked ? unlocked_highlighted : locked_highlighted;
        } else {
            color = level_unlocked ? unlocked_regular : locked_regular;
        }

        attron(color);
        const std::size_t size_t_i = static_cast<std::size_t>(i);
        mvprintw(y, x, "%c%c", levels[size_t_i][7], levels[size_t_i][8]); // levels/XX.txt, xx in pos 7 and 8
        attroff(color);
    }
}

