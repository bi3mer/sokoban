#include "game.hpp"
#include "application_state.hpp"
#include "constants.hpp"
#include "key_macros.hpp"
#include "sokoban.hpp"
#include "log.hpp"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <ncurses.h>
#include <print>
#include <utility>

void Game::on_enter() {
    Log::info("Game :: on_enter");
    app_state->moves = 0;
    app_state->start_time = std::chrono::steady_clock::now();
    commands.clear();
}

void Game::on_exit() {
    Log::info("Game :: on_exit");
    sokoban_restart(app_state->game_state);
    commands.clear();
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
            sokoban_restart(app_state->game_state);
            commands.clear();
            on_enter();
            break;
        case 'U':
        case 'u':
            if (commands.count > 0) {
                app_state->moves = std::max(app_state->moves-1, 0);
                sokoban_undo(app_state->game_state, commands.pop_back());
            }
            break;
        CASE_Q_KEYS
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
            Log::info("game :: unlocked level: {}", app_state->level_data.size());

            app_state->level_data.push_back({
                .moves = app_state->moves,
                .seconds_played = seconds_played
            });

            Log::info("game :: adding new level data to application state.");
        } else {
            // player replaying level, update moves and seconds played
            LevelData& ld = app_state->level_data[static_cast<std::size_t>(app_state->selected_index)];

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


            Log::info("game :: moves={}", ld.moves);
            Log::info("game :: seconds_played={}", ld.seconds_played);
        }

        app_state->save(); // auto-save

        // This is pretty terrible... add a game over application_state->game_state or something
        render();
        getch(); // keypress so player can see game over application_state->game_state

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

    Sokoban& game = app_state->game_state;

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    move(2, (max_x - 7) / 2);
    printw("Sokoban");

    const int start_x = (max_x - static_cast<int>(game.columns)) / 2;
    const int start_y = (max_y - static_cast<int>(game.rows)) / 2;

    Point p;
    for (p.y = 0; p.y < game.rows; ++p.y) {
        const bool player_y = game.player.y == p.y;

        for(p.x = 0; p.x < game.columns; ++p.x) {
            move(start_y + p.y, start_x + p.x);

            if (player_y && game.player.x == p.x) {
                _ui_draw_char('@');
            } else if (game.solids[sokoban_index(game, p)]) {
                _ui_draw_char('X');
            } else {
                const bool is_block = game.blocks[sokoban_index(game, p)];
                const bool is_switch = game.switches[sokoban_index(game, p)];

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