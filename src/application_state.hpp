#pragma once

#include "sokoban.hpp"
#include <cstddef>
#include <ctime>
#include <vector>

struct LevelData {
    std::size_t moves = 0;
    double seconds_played = 0;
};

struct ApplicationState {
    bool running = true;
    int selected_index = 0;

    std::size_t moves = 0;
    clock_t start_time;
    Sokoban game_state;

    std::vector<LevelData> level_data;

    ApplicationState();
    ~ApplicationState();

    void save();
};