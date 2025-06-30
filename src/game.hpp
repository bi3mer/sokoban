#pragma once

#include "sokoban.hpp"
#include "state.hpp"

struct Game : State {
    Sokoban state;
    State* menu;

    ~Game();
    State* update() override;
    void render() const override;
};