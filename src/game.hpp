#pragma once

#include "level_progression.hpp"
#include "sokoban.hpp"
#include "state.hpp"

struct LevelProgression;
struct Game : State {
    Sokoban state;
    LevelProgression* level_progression;

    ~Game();
    State* update() override;
    void render() const override;
};