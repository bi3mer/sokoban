#pragma once

#include "state.hpp"
#include <cstddef>

struct Menu : State {
    std::size_t user_selection = 0;
    State* level_progression;
    State* instructions;

    State* update() override;
    void render() const override;
};