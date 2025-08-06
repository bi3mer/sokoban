#include "menu.hpp"
#include "constants.hpp"
#include "key_macros.hpp"
#include "log.hpp"
#include "ncurses.h"
#include <cstddef>
#include <cstdlib>

const std::size_t MENU_SIZE = 3;
const char* MENU_OPTIONS[MENU_SIZE] = {
    "Play",
    "Instructions",
    "Exit",
};

void Menu::on_enter() {
    Log::info("Menu :: on_enter");
}

void Menu::on_exit() {
    Log::info("Menu :: on_exit");
}

State* Menu::update() {
    switch (getch()) {
        CASE_SELECT_KEYS
            switch(user_selection) {
            case 0:
                Log::info("menu :: goto level progression");
                return level_progression;
            case 1:
                Log::info("menu :: goto instructions");
                return instructions;
            case 2:
                Log::info("menu :: player exiting");
                app_state->running = false;
                break;
            default:
                Log::warning("invalid menu selection index: {}", user_selection);
                user_selection = 0;
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
            Log::info("menu :: player quitting");
            app_state->running = false;
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
        move(y, (max_x - static_cast<int>(strlen(MENU_OPTIONS[i]))) / 2);
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