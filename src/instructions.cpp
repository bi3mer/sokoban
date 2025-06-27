#include "instructions.hpp"
#include "menu.hpp"
#include <cstddef>
#include <cstring>
#include <ncurses.h>

const std::size_t INSTRUCTIONS_SIZE = 5;
const char* INSTRUCTIONS[INSTRUCTIONS_SIZE] = {
    "Move blocks ('b') over switches ('.') to win!",
    "W to move up",
    "A to move left",
    "S to move down",
    "D to move right",
};

State* Instructions::update() {
    const int user_input = getch();
    if (user_input == '\n' || user_input == ' ') {
        return menu;
    }

    return this;
}

void Instructions::render() const {
    clear();

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    int y = max_y / 3;
    for (std::size_t i = 0; i < INSTRUCTIONS_SIZE; ++i, ++y) {
        move(y, (max_x - strlen(INSTRUCTIONS[i])) / 2);
        printw(INSTRUCTIONS[i]);
    }

    move(y+2, (max_x - 19) / 2);
    const auto cp = COLOR_PAIR(UI_HIGHLIGHTED);
    attron(cp);
    printw("Return to Main Menu");
    attroff(cp);
}
