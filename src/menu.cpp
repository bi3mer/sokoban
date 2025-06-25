#include "menu.hpp"
#include "ncurses.h"
#include "ui.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>

const std::size_t MENU_SIZE = 2;
const char* MENU_OPTIONS[MENU_SIZE] = {
    "Play",
    "Instructions"
};

State Menu::update(const int user_input) {
    switch (user_input) {
        case ' ':
        case '\n':
            switch(user_selection) {
                case 0:
                    return State::Game;
                case 1:
                    // @TODO: put in instructions
                    break;
                default:
                    ui_close();
                    std::cerr << "Invalid menu selection index " << user_selection << std::endl;
                    exit(1);
            }
            break;
        case 'W':
        case 'w':
        case 3:
            user_selection = (user_selection - 1) % MENU_SIZE;
            break;
        case 'S':
        case 's':
        case 2:
            user_selection = (user_selection + 1) % MENU_SIZE;
            break;
    }

    return State::Menu;
}

void Menu::render() {
    clear();
    move(0,0);

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    const auto regular = COLOR_PAIR('@');
    const auto highlighted = COLOR_PAIR('B');

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