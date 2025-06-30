#include <cassert>
#include <cstring>
#include <ncurses.h>

#include "constants.hpp"
#include "game.hpp"
#include "instructions.hpp"
#include "level_progression.hpp"
#include "menu.hpp"
#include "state.hpp"

bool RUNNING = true;

int main(int argc, char* argv[]) {
    /////////////////////////////////////////////////////////
    /// Initialize ncurses
    initscr();
    curs_set(0); // invisible cursor
    keypad(stdscr, TRUE);

    // initialize colors
    start_color();
    init_pair('@', COLOR_WHITE, COLOR_BLACK);
    init_pair('b', COLOR_RED, COLOR_BLACK);
    init_pair('B', COLOR_GREEN, COLOR_BLACK);
    init_pair('.', COLOR_RED, COLOR_BLACK);
    init_pair('X', COLOR_WHITE, COLOR_WHITE);
    init_pair(COLOR_REGULAR, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_HIGHLIGHTED, COLOR_BLACK, COLOR_WHITE);

    /////////////////////////////////////////////////////////
    /// Set up state machine
    Menu menu;
    Instructions instructions;
    LevelProgression level_progression;
    Game game;

    // transitions
    menu.level_progression = &level_progression;
    menu.instructions = &instructions;

    level_progression.menu = &menu;
    level_progression.game = &game;

    instructions.menu = &menu;
    game.menu = &menu;

    // set first state
    State* state = &menu;

    /////////////////////////////////////////////////////////
    // loop
    state->render();
    while (RUNNING) {
        state = state->update();
        state->render();
    }

    // shutdown
    endwin();

    return 0;
}