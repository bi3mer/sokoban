#include "ui.hpp"
#include "ncurses.h"

void ui_init() {
    initscr();
    curs_set(0); // invisible cursor
}

void ui_close() {
    //------------------------------------------
    // this will be removed when we get to menus
    move(0, 0);
    printw("You won!");
    getch(); // pause before ending
    //------------------------------------------

    endwin();
}

char ui_get_char() {
    return getch();
}

void ui_render_sokoban(const Sokoban & state) {
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
                addch('@');
            } else if (state_is_solid(state, p)) {
                addch('X');
            } else {
                const bool is_block = state_is_block(state, p);
                const bool is_switch = state_is_switch(state, p);

                if (is_block && is_switch) {
                    addch('B');
                } else if (is_block) {
                    addch('b');
                } else if(is_switch) {
                    addch('.');
                }
            }

        }
    }
}