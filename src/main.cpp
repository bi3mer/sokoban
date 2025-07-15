#include <cassert>
#include <cstring>
#include <iostream>
#include <ncurses.h>

#include "PooledLinkedList.hpp"
#include "constants.hpp"
#include "game.hpp"
#include "instructions.hpp"
#include "level_progression.hpp"
#include "log.hpp"
#include "menu.hpp"
#include "state.hpp"


int main(int argc, char* argv[]) {
    // @TODO: we need to fix this case for our linked list
    // PooledLinkedList<int> ll;
    // for(int i = 0; i < 10; ++i) {
    //     ll.push_front(i);
    // }

    // while (ll.front != nullptr) {
    //     std::cout << ll.pop_front() << std::endl;
    // }

    // return 0;
    /////////////////////////////////////////////////////////
    /// Initialize logger and state
    Log::init();
    ApplicationState application_state;

    /////////////////////////////////////////////////////////
    /// Initialize ncurses
    Log::info("initializing ncurses.");
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

    init_pair(COLOR_UNLOCKED, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_UNLOCKED_HIGHLIGHTED, COLOR_BLACK, COLOR_GREEN);

    init_pair(COLOR_LOCKED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_LOCKED_HIGHLIGHTED, COLOR_BLACK, COLOR_RED);

    /////////////////////////////////////////////////////////
    /// Set up state machine
    Log::info("setting up state machine");
    Menu menu(&application_state);
    Instructions instructions(&application_state);
    LevelProgression level_progression(&application_state);
    Game game(&application_state);

    // transitions
    menu.level_progression = &level_progression;
    menu.instructions = &instructions;

    level_progression.menu = &menu;
    level_progression.game = &game;

    instructions.menu = &menu;
    game.level_progression = &level_progression;

    // set first state
    State* state = &menu;

    /////////////////////////////////////////////////////////
    // loop
    Log::info("beginning game loop.");
    state->render();
    while (application_state.running) {
        state = state->update();
        state->render();
    }

    // shutdown
    endwin();
    Log::close();

    return 0;
}