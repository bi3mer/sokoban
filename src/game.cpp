#include "game.hpp"
#include "key_macros.hpp"
#include "sokoban.hpp"
#include "log.hpp"

#include <ctime>
#include <format>
#include <ncurses.h>

Game::~Game() {
    sokoban_free(app_state->game_state);
}

State* Game::update() {
    switch (getch()) {
        CASE_UP_KEYS
            ++app_state->moves;
            sokoban_update(app_state->game_state, {0,-1});
            break;
        CASE_LEFT_KEYS
            ++app_state->moves;
            sokoban_update(app_state->game_state, {-1,0});
            break;
        CASE_DOWN_KEYS
            ++app_state->moves;
            sokoban_update(app_state->game_state, {0,1});
            break;
        CASE_RIGHT_KEYS
            ++app_state->moves;
            sokoban_update(app_state->game_state, {1,0});
            break;
        case 'R':
        case 'r':
            Log::info("game :: restart");
            app_state->moves = 0;
            app_state->start_time = clock();
            sokoban_restart(app_state->game_state);
            break;
        CASE_Q_KEYS
            sokoban_restart(app_state->game_state);
            Log::info("game :: player quit");
            Log::info("game :: goto level progression");
            return level_progression;
        default:
            break;
    }

    if (sokoban_game_over(app_state->game_state)) {
        Log::info("game :: player won");

        app_state->end_time = static_cast<double>(clock() - app_state->start_time) / CLOCKS_PER_SEC;

        if (app_state->selected_index == app_state->max_level_beaten) {
            ++app_state->max_level_beaten;
            Log::info(
               std::format(
                   "game :: unlocked level: {}",
                   app_state->max_level_beaten
               ).c_str()
            );

            app_state->save(); // auto-save
        }

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