#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>

#include "game.hpp"
#include "instructions.hpp"
#include "menu.hpp"
#include "sokoban.hpp"
#include "state.hpp"

bool RUNNING = true;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "A path to a single Sokobon level must be provided." << std::endl;
        return 1;
    }

    std::ifstream level_file(argv[1]);
    if (!level_file.good()) {
        std::cerr << "Could not open file: " << argv[1] << std::endl;
        return 1;
    }

    std::vector<std::string> level;
    std::string line;
    while(getline(level_file, line)) {
        level.push_back(line);
    }

    level_file.close();

    const std::size_t num_columns = level[0].size();
    for(std::size_t i = 1; i < level.size(); ++i) {
        assert(level[i].size() == num_columns);
    }

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
    init_pair(UI_REGULAR, COLOR_WHITE, COLOR_BLACK);
    init_pair(UI_HIGHLIGHTED, COLOR_BLACK, COLOR_WHITE);

    /////////////////////////////////////////////////////////
    /// Set up state machine
    Menu menu;
    Instructions instructions;
    Game game;
    sokoban_init_from_level(game.state, level);

    // transitions
    menu.game = &game;
    menu.instructions = &instructions;
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
    sokoban_free(game.state);
    endwin();

    return 0;
}