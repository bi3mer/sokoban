#pragma once

#include "sokoban.hpp"
#include <chrono>
#include <cstddef>
#include <vector>

struct LevelData {
    int moves = 0;
    double seconds_played = 0;
};

struct ApplicationState {
    const char* game_over_move_message = nullptr;
    const char* game_over_time_message = nullptr;
    bool running = true;
    int selected_index = 0;
    double seconds_played_message;

    int moves = 0;
    std::chrono::steady_clock::time_point start_time;
    Sokoban game_state;

    std::vector<LevelData> level_data;

    ApplicationState();
    ~ApplicationState();
    void save();
};