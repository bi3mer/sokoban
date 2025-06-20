#include "ui.hpp"
#include "ncurses.h"


void ui_init() {
    initscr();
    curs_set(0); // invisible cursor

    start_color();
    init_pair('@', COLOR_WHITE, COLOR_BLACK);
    init_pair('b', COLOR_RED, COLOR_BLACK);
    init_pair('B', COLOR_GREEN, COLOR_BLACK);
    init_pair('.', COLOR_RED, COLOR_BLACK);
    init_pair('X', COLOR_WHITE, COLOR_WHITE);
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


void ui_render_menu() {
    clear();
    move(0,0);

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    move(max_y / 3, (max_x - 7) / 2);

    const auto cp = COLOR_PAIR('B');
    attron(cp);
    printw("Sokoban");

    move(max_y / 2, (max_x - 22) / 2);
    printw("Press 'enter' to start");
    attroff(cp);
}

inline void _ui_draw_char(const char c) {
    const auto cp = COLOR_PAIR(c);
    attron(cp);
    addch(c);
    attroff(cp);
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