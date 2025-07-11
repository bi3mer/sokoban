#pragma once

#include "sokoban.hpp"
#include <ctime>

struct ApplicationState {
    bool running = true;
    int selected_index = 0;
    int max_level_beaten = 0;

    int moves = 0;
    clock_t start_time;
    double end_time = 0;

    Sokoban game_state;

    ApplicationState();
    ~ApplicationState();

    void save();
};