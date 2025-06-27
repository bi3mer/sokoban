#include "menu.hpp"
#include "ncurses.h"
#include <cstddef>
#include <cstdlib>
#include <iostream>

extern bool RUNNING;

const std::size_t MENU_SIZE = 3;
const char* MENU_OPTIONS[MENU_SIZE] = {
    "Play",
    "Instructions",
    "Exit",
};

State* Menu::update() {
    switch (getch()) {
        case ' ':
        case '\n':
            switch(user_selection) {
                case 0:
                    return game;
                case 1:
                    return instructions;
                case 2:
                    RUNNING = false;
                    break;
                default:
                    // @TODO: don't error out, just stop running
                    std::cerr << "Invalid menu selection index " << user_selection << std::endl;
                    RUNNING = false;
                    break;
            }
            break;
        case 'W':
        case 'w':
        case KEY_UP:
            user_selection = (user_selection - 1) % MENU_SIZE;
            break;
        case 'S':
        case 's':
        case KEY_DOWN:
            user_selection = (user_selection + 1) % MENU_SIZE;
            break;
        case 'Q':
        case 'q':
            RUNNING = false;
            break;
    }

    return this;
}

void Menu::render() const {
    clear();
    move(0,0);

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    const auto regular = COLOR_PAIR(UI_REGULAR);
    const auto highlighted = COLOR_PAIR(UI_HIGHLIGHTED);

    move(max_y / 3, (max_x - 7) / 2);
    attron(regular);
    printw("Sokoban");
    attroff(regular);

    int y = max_y / 2;
    for (std::size_t i = 0; i < MENU_SIZE; ++i) {
        move(y, (max_x - strlen(MENU_OPTIONS[i])) / 2);
        if (i == user_selection) {
            attron(highlighted);
            printw(MENU_OPTIONS[i]);
            attroff(highlighted);
        } else {
            attron(regular);
            printw(MENU_OPTIONS[i]);
            attroff(regular);
        }

        y += 2;
    }
}