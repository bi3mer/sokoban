#include "menu.hpp"
#include "constants.hpp"
#include "key_macros.hpp"
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
        CASE_SELECT_KEYS
            switch(user_selection) {
                case 0:
                    return level_progression;
                case 1:
                    return instructions;
                case 2:
                    RUNNING = false;
                    break;
                default:
                    std::cerr << "Invalid menu selection index " << user_selection << std::endl;
                    RUNNING = false;
                    break;
            }
            break;
        CASE_UP_KEYS
            user_selection = (user_selection - 1) % MENU_SIZE;
            break;
        CASE_DOWN_KEYS
            user_selection = (user_selection + 1) % MENU_SIZE;
            break;
        CASE_Q_KEYS
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

    const auto regular = COLOR_PAIR(COLOR_REGULAR);
    const auto highlighted = COLOR_PAIR(COLOR_HIGHLIGHTED);

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