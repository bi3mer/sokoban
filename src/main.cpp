#include <cassert>
#include <cstring>
#include <ncurses.h>

#include "application_state.hpp"
#include "constants.hpp"
#include "game.hpp"
#include "instructions.hpp"
#include "level_progression.hpp"
#include "log.hpp"
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

    init_pair(COLOR_UNLOCKED, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_UNLOCKED_HIGHLIGHTED, COLOR_BLACK, COLOR_GREEN);

    init_pair(COLOR_LOCKED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_LOCKED_HIGHLIGHTED, COLOR_BLACK, COLOR_RED);

    /////////////////////////////////////////////////////////
    // Set up systems
    Log::init();

    // Set up state machine
    Log::info("Initializing states");
    ApplicationState application_state;
    Log::info("Initializing states");
    Menu menu(application_state);
    Instructions instructions(application_state);
    LevelProgression level_progression(application_state);
    Game game(application_state);
    Log::info("Initializing states");

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
    Log::info("Starting state...");
    state->render();
    while (RUNNING) {
        state = state->update();
        state->render();
    }

    // shutdown
    endwin();
    application_state.save();
    Log::close();

    return 0;
}