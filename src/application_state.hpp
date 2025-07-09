#pragma once

#include "sokoban.hpp"
#include <ctime>

struct ApplicationState {
    bool running = true;
    int selected_index = 0;
    int max_level_beaten = 0;

    int moves = 0;
    clock_t start_time;

    Sokoban game_state;
};