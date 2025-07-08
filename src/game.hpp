#pragma once

#include "application_state.hpp"
#include "level_progression.hpp"
#include "sokoban.hpp"
#include "state.hpp"
#include <ctime>

struct LevelProgression;
struct Game : State {
    clock_t start_time;
    int moves;
    Sokoban state;
    LevelProgression* level_progression;

    Game(ApplicationState& state) : State(state) {};

    ~Game();
    State* update() override;
    void render() const override;
};