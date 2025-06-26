#pragma once

#include "sokoban.hpp"
#include "state.hpp"

struct Game : State {
    Sokoban state;
    State* menu;

    State* update() override;
    void render() override;
};