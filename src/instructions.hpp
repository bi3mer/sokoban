#pragma once

#include "state.hpp"
#include <cstddef>

struct Instructions : State {
    std::size_t user_selection = 0;
    State* menu;

    State* update() override;
    void render() const override;
};