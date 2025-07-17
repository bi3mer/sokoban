#include "game.hpp"
#include "application_state.hpp"
#include "constants.hpp"
#include "key_macros.hpp"
#include "sokoban.hpp"
#include "log.hpp"

#include <algorithm>
#include <chrono>
#include <format>
#include <ncurses.h>
#include <utility>

Game::~Game() {
    sokoban_free(app_state->game_state);
}

State* Game::update() {
    switch (getch()) {
        CASE_UP_KEYS
            ++app_state->moves;
            commands.push_back(sokoban_update(app_state->game_state, {0, -1}));
            break;
        CASE_LEFT_KEYS
            ++app_state->moves;
            commands.push_back(sokoban_update(app_state->game_state, {-1, 0}));
            break;
        CASE_DOWN_KEYS
            ++app_state->moves;
            commands.push_back(sokoban_update(app_state->game_state, {0, 1}));
            break;
        CASE_RIGHT_KEYS
            ++app_state->moves;
            commands.push_back(sokoban_update(app_state->game_state, {1, 0}));
            break;
        case 'R':
        case 'r':
            Log::info("game :: restart");
            app_state->moves = 0;
            app_state->start_time = std::chrono::steady_clock::now();
            commands.clear();
            sokoban_restart(app_state->game_state);
            break;
        case 'U':
        case 'u':
            if (commands.size > 0) {
                app_state->moves = std::max(app_state->moves-1, 0);
                sokoban_undo(app_state->game_state, commands.pop_back());
            }
            break;
        CASE_Q_KEYS
            sokoban_restart(app_state->game_state);
            commands.clear();
            Log::info("game :: player quit");
            Log::info("game :: goto level progression");
            return level_progression;
        default:
            break;
    }

    if (sokoban_game_over(app_state->game_state)) {
        Log::info("game :: player won");
        const auto  end = std::chrono::steady_clock::now();
        const double ms_played = std::chrono::duration_cast<std::chrono::milliseconds>(
            end - app_state->start_time
        ).count();
        const double seconds_played = std::ceil(ms_played * 0.001 * 100.0) / 100.0;

        if (app_state->selected_index == app_state->level_data.size()) {
            Log::info(
               std::format(
                   "game :: unlocked level: {}",
                   app_state->level_data.size()
               ).c_str()
            );

            app_state->level_data.push_back({
                .moves = app_state->moves,
                .seconds_played = seconds_played
            });

            Log::info("game :: adding new level data to application state.");
        } else {
            // player replaying level, update moves and seconds played
            LevelData& ld = app_state->level_data[app_state->selected_index];

            if (ld.moves > app_state->moves) {
                std::swap(ld.moves, app_state->moves);
                app_state->game_over_move_message = MOVE_MESSAGE;
            } else {
                app_state->game_over_move_message = nullptr;
            }

            if (ld.seconds_played > seconds_played) {
                app_state->seconds_played_message = ld.seconds_played;
                ld.seconds_played = seconds_played;

                app_state->game_over_time_message = TIME_MESSAGE;
            } else {
                app_state->game_over_time_message = nullptr;
            }

            // ld.moves = std::min(ld.moves, app_state->moves);
            // ld.seconds_played = std::min(ld.seconds_played, seconds_played);

            Log::info(std::format("game :: moves={}", ld.moves).c_str());
            Log::info(std::format("game :: seconds_played={}", ld.seconds_played).c_str());
        }

        app_state->save(); // auto-save

        // This is pretty terrible... add a game over application_state->game_state or something
        render();
        getch(); // keypress so player can see game over application_state->game_state
        sokoban_restart(app_state->game_state);

        Log::info("game :: goto level progression");
        return level_progression;
    }

    return this;
}

inline void _ui_draw_char(const char c) {
    const auto cp = COLOR_PAIR(c);
    attron(cp);
    addch(c);
    attroff(cp);
}

void Game::render() const {
    clear();

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    move(2, (max_x - 7) / 2);
    printw("Sokoban");

    const int start_x = (max_x - app_state->game_state.columns) / 2;
    const int start_y = (max_y - app_state->game_state.rows) / 2;

    Point p;
    for (p.y = 0; p.y < app_state->game_state.rows; ++p.y) {
        const bool player_y = app_state->game_state.player.y == p.y;

        for(p.x = 0; p.x < app_state->game_state.columns; ++p.x) {
            move(start_y + p.y, start_x + p.x);

            if (player_y && app_state->game_state.player.x == p.x) {
                _ui_draw_char('@');
            } else if (sokoban_is_solid(app_state->game_state, p)) {
                _ui_draw_char('X');
            } else {
                const bool is_block = sokoban_is_block(app_state->game_state, p);
                const bool is_switch = sokoban_is_switch(app_state->game_state, p);

                if (is_block && is_switch) {
                    _ui_draw_char('B');
                } else if (is_block) {
                    _ui_draw_char('b');
                } else if(is_switch) {
                    _ui_draw_char('.');
                }
            }

        }
    }
}