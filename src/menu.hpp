#pragma once

#include "state.hpp"
#include <cstddef>

#define UI_HIGHLIGHTED 1
#define UI_REGULAR 2

struct Menu : State {
    std::size_t user_selection = 0;
    State* game;

    State* update() override;
    void render() override;
};