#include "game.hpp"
#include "sokoban.hpp"
#include <ncurses.h>

State* Game::update() {
    switch (getch()) {
        case 'W':
        case 'w':
        case KEY_UP:
            sokoban_update(state, {0,-1});
            break;
        case 'A':
        case 'a':
        case KEY_LEFT:
            sokoban_update(state, {-1,0});
            break;
        case 'S':
        case 's':
        case KEY_DOWN:
            sokoban_update(state, {0,1});
            break;
        case 'D':
        case 'd':
        case KEY_RIGHT:
            sokoban_update(state, {1,0});
            break;
        case 'R':
        case 'r':
            sokoban_restart(state);
            break;
        case 'Q':
        case 'q':
            sokoban_restart(state);
            return menu;
        default:
            break;
    }

    if (sokoban_game_over(state)) {
        // This is pretty terrible... add a game over state or something
        render();
        getch(); // keypress so player can see game over state
        sokoban_restart(state);
        return menu;
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

    const int start_x = (max_x - state.columns) / 2;
    const int start_y = (max_y - state.rows) / 2;

    Point p;
    for (p.y = 0; p.y < state.rows; ++p.y) {
        const bool player_y = state.player.y == p.y;

        for(p.x = 0; p.x < state.columns; ++p.x) {
            move(start_y + p.y, start_x + p.x);

            if (player_y && state.player.x == p.x) {
                _ui_draw_char('@');
            } else if (sokoban_is_solid(state, p)) {
                _ui_draw_char('X');
            } else {
                const bool is_block = sokoban_is_block(state, p);
                const bool is_switch = sokoban_is_switch(state, p);

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