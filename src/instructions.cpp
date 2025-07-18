#include "instructions.hpp"
#include "constants.hpp"
#include "key_macros.hpp"
#include "log.hpp"

#include <cstring>
#include <ncurses.h>

const std::size_t INSTRUCTIONS_SIZE = 10;
const char* INSTRUCTIONS[INSTRUCTIONS_SIZE] = {
    "Move blocks ('b') over switches ('.') to win!",
    "",
    "W to move up",
    "A to move left",
    "S to move down",
    "D to move right",
    " ",
    "U to undo",
    "R to restart",
    "Q to quit",
};


void Instructions::on_enter() {
    Log::info("Instructions :: on_enter");
}

void Instructions::on_exit() {
    Log::info("Instructions :: on_enter");
}

State* Instructions::update() {
    switch(getch()) {
        CASE_SELECT_KEYS
            Log::info("level progression :: goto menu");
            return menu;
        default:
            return this;
    }
}

void Instructions::render() const {
    clear();

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    int y = max_y / 5;

    for(std::size_t i = 0; i < INSTRUCTIONS_SIZE; ++i, ++y) {
        move(y, (max_x - static_cast<int>(strlen(INSTRUCTIONS[i]))) / 2);
        printw(INSTRUCTIONS[i]);
    }

    move(y+2, (max_x - 19) / 2);
    const auto highlighted = COLOR_PAIR(COLOR_HIGHLIGHTED);
    attron(highlighted);
    printw("Return to Main Menu");
    attroff(highlighted);
}